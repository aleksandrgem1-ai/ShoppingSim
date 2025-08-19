// BuildManagerSubsystem.cpp
#include "Subsystems/BuildManagerSubsystem.h"
#include "Actors/StoreZoneActor.h"
#include "Data/StoreZoneData.h"
#include "Subsystems/EconomySubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/BuildModePawn.h"
#include "Settings/SimSettings.h"
#include "Utils/AimTraceService.h"

void UBuildManagerSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
  Super::Initialize(Collection);

  PlayerMappingContext = Cast<UInputMappingContext>(
      StaticLoadObject(UInputMappingContext::StaticClass(), nullptr,
                       TEXT("/Game/AssetInput/IMC_Default.IMC_Default")));
  BuildModeMappingContext = Cast<UInputMappingContext>(
      StaticLoadObject(UInputMappingContext::StaticClass(), nullptr,
                       TEXT("/Game/AssetInput/IMC_BuildMode.IMC_BuildMode")));
  TestZoneToBuild = Cast<UStoreZoneData>(
      StaticLoadObject(UStoreZoneData::StaticClass(), nullptr,
                       TEXT("/Game/Data/Zones/DA_TestZone.DA_TestZone")));
  BuildModePawnClass = ABuildModePawn::StaticClass();
}

void UBuildManagerSubsystem::ToggleBuildMode() {
  bIsInBuildMode = !bIsInBuildMode;
  if (bIsInBuildMode)
    EnterBuildMode();
  else
    ExitBuildMode();
}

void UBuildManagerSubsystem::EnterBuildMode() {
  APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (!PC || !BuildModePawnClass)
    return;

  OriginalPlayerPawn = PC->GetPawn();
  if (IsValid(OriginalPlayerPawn))
    OriginalPlayerPawn->DisableInput(PC);

  FActorSpawnParameters SpawnParams;
  SpawnParams.Owner = PC;
  SpawnParams.SpawnCollisionHandlingOverride =
      ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

  const FVector SpawnLoc = IsValid(OriginalPlayerPawn)
                               ? OriginalPlayerPawn->GetActorLocation()
                               : FVector::ZeroVector;
  const FRotator SpawnRot = IsValid(OriginalPlayerPawn)
                                ? OriginalPlayerPawn->GetActorRotation()
                                : FRotator::ZeroRotator;

  SpawnedBuildPawn = GetWorld()->SpawnActor<APawn>(BuildModePawnClass, SpawnLoc,
                                                   SpawnRot, SpawnParams);
  if (!IsValid(SpawnedBuildPawn))
    return;

  PC->Possess(SpawnedBuildPawn);

  if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
          ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
              PC->GetLocalPlayer())) {
    if (PlayerMappingContext)
      Subsystem->RemoveMappingContext(PlayerMappingContext);
    if (BuildModeMappingContext)
      Subsystem->AddMappingContext(BuildModeMappingContext, /*Priority*/ 1);
  }
}

void UBuildManagerSubsystem::ExitBuildMode() {
  CancelPlacement();

  APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (!PC)
    return;

  if (IsValid(SpawnedBuildPawn) && PC->GetPawn() == SpawnedBuildPawn) {
    SpawnedBuildPawn->Destroy();
    SpawnedBuildPawn = nullptr;
  }

  if (IsValid(OriginalPlayerPawn)) {
    PC->Possess(OriginalPlayerPawn);
    OriginalPlayerPawn->EnableInput(PC);
    OriginalPlayerPawn = nullptr;
  }

  if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
          ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
              PC->GetLocalPlayer())) {
    if (BuildModeMappingContext)
      Subsystem->RemoveMappingContext(BuildModeMappingContext);
    if (PlayerMappingContext)
      Subsystem->AddMappingContext(PlayerMappingContext, /*Priority*/ 0);
  }
}

void UBuildManagerSubsystem::StartPlacement() {
  if (GhostActor) {
    GhostActor->Destroy();
    GhostActor = nullptr;
  }

  if (TestZoneToBuild) {
    GhostActor =
        GetWorld()->SpawnActor<AStoreZoneActor>(AStoreZoneActor::StaticClass());
    if (GhostActor) {
      GhostActor->ZoneData = TestZoneToBuild;
      GhostActor->PostInitializeComponents();
    }
  }
}

void UBuildManagerSubsystem::UpdateGhostActorTransform() {
  if (!GhostActor)
    return;

  const USimSettings *Settings = GetDefault<USimSettings>();
  const float TraceDistance = Settings ? Settings->TraceDistanceBuild : 15000.f;
  const ECollisionChannel TraceChannel =
      Settings ? Settings->TraceChannelBuild.GetValue() : ECC_Visibility;
  const float GridSize = Settings ? Settings->GridSize : 100.f;
  const float ZOffset = Settings ? Settings->PlacementZOffset : 0.f;
  const bool bAlignToSurface = Settings ? Settings->bAlignToSurface : false;

  APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (!PC)
    return;

  TArray<const AActor *> Ignore;
  Ignore.Add(GhostActor);
  if (APawn *Pawn = PC->GetPawn())
    Ignore.Add(Pawn);

  FHitResult HitResult;
  const bool bHit = FAimTraceService::TraceFromScreenCenter(
      GetWorld(), PC, TraceDistance, TraceChannel, Ignore, HitResult, false);

  FVector Start, End;
  FAimTraceService::MakeScreenCenterRay(PC, TraceDistance, Start, End);

  FVector TargetLocation = bHit ? HitResult.Location : End;
  if (bHit && bAlignToSurface) {
    // Выравниваем по нормали — вращаем вокруг оси, чтобы Z совпал с нормалью
    const FRotator SurfaceRot = HitResult.Normal.ToOrientationRotator();
    const FRotator UserYaw(0.f, CurrentPlacementRotation, 0.f);
    const FRotator FinalRot = FRotator(SurfaceRot.Pitch, UserYaw.Yaw, 0.f);
    TargetLocation.Z += ZOffset;
    GhostActor->SetActorTransform(FTransform(FinalRot, TargetLocation));
    return;
  }

  if (bIsGridSnappingEnabled) {
    TargetLocation.X = FMath::GridSnap(TargetLocation.X, GridSize);
    TargetLocation.Y = FMath::GridSnap(TargetLocation.Y, GridSize);
  }
  TargetLocation.Z += ZOffset;

  const FRotator TargetRotation(0.f, CurrentPlacementRotation, 0.f);
  GhostActor->SetActorTransform(FTransform(TargetRotation, TargetLocation));
}

void UBuildManagerSubsystem::ConfirmPlacement() {
  if (!GhostActor || !TestZoneToBuild)
    return;

  if (UEconomySubsystem *Economy =
          GetGameInstance()->GetSubsystem<UEconomySubsystem>()) {
    if (Economy->TrySpendMoney(TestZoneToBuild->Cost)) {
      AStoreZoneActor *NewZone = GetWorld()->SpawnActor<AStoreZoneActor>(
          AStoreZoneActor::StaticClass(), GhostActor->GetActorTransform());
      if (NewZone) {
        NewZone->ZoneData = TestZoneToBuild;
        NewZone->PostInitializeComponents();
      }
      CurrentPlacementRotation = 0.f;
    }
  }
}

void UBuildManagerSubsystem::CancelPlacement() {
  if (GhostActor) {
    GhostActor->Destroy();
    GhostActor = nullptr;
  }
}

void UBuildManagerSubsystem::SelectObjectForMove(AStoreZoneActor *ZoneToMove) {
  if (!ZoneToMove || GhostActor)
    return;

  TestZoneToBuild = ZoneToMove->ZoneData;
  StartPlacement();

  if (UEconomySubsystem *Economy =
          GetGameInstance()->GetSubsystem<UEconomySubsystem>()) {
    if (ZoneToMove->ZoneData)
      Economy->AddMoney(ZoneToMove->ZoneData->Cost);
  }

  ZoneToMove->Destroy();
}

void UBuildManagerSubsystem::HandleDestruction(AStoreZoneActor *ZoneToDestroy) {
  if (!ZoneToDestroy || !ZoneToDestroy->ZoneData)
    return;

  if (UEconomySubsystem *Economy =
          GetGameInstance()->GetSubsystem<UEconomySubsystem>()) {
    const int32 RefundAmount =
        FMath::RoundToInt(ZoneToDestroy->ZoneData->Cost * 0.75f);
    Economy->AddMoney(RefundAmount);
  }

  ZoneToDestroy->Destroy();
}

void UBuildManagerSubsystem::AddRotation(float Angle) {
  CurrentPlacementRotation += Angle;
}

void UBuildManagerSubsystem::ToggleGridSnapping() {
  bIsGridSnappingEnabled = !bIsGridSnappingEnabled;
}
