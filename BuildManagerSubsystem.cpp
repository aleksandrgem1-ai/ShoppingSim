// BuildManagerSubsystem.cpp (ИСПРАВЛЕННАЯ ПОЛНАЯ ВЕРСИЯ)

#include "BuildManagerSubsystem.h"
#include "BuildModePawn.h"
#include "EconomySubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "StoreZoneActor.h"
#include "StoreZoneData.h"

void UBuildManagerSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
  Super::Initialize(Collection);

  // --- ИСПРАВЛЕННЫЕ ПУТИ ---
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

  if (bIsInBuildMode) {
    EnterBuildMode();
  } else {
    ExitBuildMode();
  }
}

void UBuildManagerSubsystem::EnterBuildMode() {
  APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (!PC || !BuildModePawnClass)
    return;

  OriginalPlayerPawn = PC->GetPawn();
  if (OriginalPlayerPawn) {
    OriginalPlayerPawn->DisableInput(PC);
  }

  FActorSpawnParameters SpawnParams;
  SpawnParams.Owner = PC;
  APawn *BuildPawn = GetWorld()->SpawnActor<APawn>(
      BuildModePawnClass, OriginalPlayerPawn->GetActorLocation(),
      OriginalPlayerPawn->GetActorRotation(), SpawnParams);
  PC->Possess(BuildPawn);

  if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
          ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
              PC->GetLocalPlayer())) {
    if (PlayerMappingContext)
      Subsystem->RemoveMappingContext(PlayerMappingContext);
    if (BuildModeMappingContext)
      Subsystem->AddMappingContext(BuildModeMappingContext, 0);
  }
}

void UBuildManagerSubsystem::ExitBuildMode() {
  CancelPlacement();

  APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (!PC)
    return;

  if (APawn *CurrentPawn = PC->GetPawn()) {
    CurrentPawn->Destroy();
  }

  if (OriginalPlayerPawn) {
    PC->Possess(OriginalPlayerPawn);
    OriginalPlayerPawn->EnableInput(PC);
    OriginalPlayerPawn =
        nullptr; // Сбрасываем указатель, чтобы избежать проблем
  }

  if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
          ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
              PC->GetLocalPlayer())) {
    if (BuildModeMappingContext)
      Subsystem->RemoveMappingContext(BuildModeMappingContext);
    if (PlayerMappingContext)
      Subsystem->AddMappingContext(PlayerMappingContext, 0);
  }
}

void UBuildManagerSubsystem::StartPlacement() {
  if (GhostActor) {
    GhostActor->Destroy();
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

  APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (!PC)
    return;

  FVector Start, Dir;
  PC->DeprojectMousePositionToWorld(Start, Dir);
  FVector End = Start + (Dir * 10000.f);

  FHitResult HitResult;
  FCollisionQueryParams QueryParams;
  QueryParams.AddIgnoredActor(GhostActor);
  QueryParams.AddIgnoredActor(PC->GetPawn());

  GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility,
                                       QueryParams);

  FVector TargetLocation = HitResult.bBlockingHit ? HitResult.Location : End;

  if (bIsGridSnappingEnabled) {
    const float GridSize = 100.0f;
    TargetLocation.X = FMath::GridSnap(TargetLocation.X, GridSize);
    TargetLocation.Y = FMath::GridSnap(TargetLocation.Y, GridSize);
  }

  FRotator TargetRotation(0, CurrentPlacementRotation, 0);
  GhostActor->SetActorTransform(FTransform(TargetRotation, TargetLocation));
}

void UBuildManagerSubsystem::ConfirmPlacement() {
  if (!GhostActor || !TestZoneToBuild)
    return;

  UEconomySubsystem *Economy =
      GetGameInstance()->GetSubsystem<UEconomySubsystem>();
  if (Economy && Economy->TrySpendMoney(TestZoneToBuild->Cost)) {
    AStoreZoneActor *NewZone = GetWorld()->SpawnActor<AStoreZoneActor>(
        AStoreZoneActor::StaticClass(), GhostActor->GetActorTransform());
    if (NewZone) {
      NewZone->ZoneData = TestZoneToBuild;
      NewZone->PostInitializeComponents();
    }
    CurrentPlacementRotation = 0.f;
  }
}

void UBuildManagerSubsystem::CancelPlacement() {
  if (GhostActor) {
    GhostActor->Destroy();
    GhostActor = nullptr;
  }
}

void UBuildManagerSubsystem::SelectObjectForMove(AStoreZoneActor *ZoneToMove) {
  if (!ZoneToMove || GhostActor) {
    return;
  }

  TestZoneToBuild = ZoneToMove->ZoneData;
  StartPlacement();

  UEconomySubsystem *Economy =
      GetGameInstance()->GetSubsystem<UEconomySubsystem>();
  if (Economy && ZoneToMove->ZoneData) {
    Economy->AddMoney(ZoneToMove->ZoneData->Cost);
  }

  ZoneToMove->Destroy();
}

void UBuildManagerSubsystem::HandleDestruction(AStoreZoneActor *ZoneToDestroy) {
  if (!ZoneToDestroy || !ZoneToDestroy->ZoneData)
    return;

  UEconomySubsystem *Economy =
      GetGameInstance()->GetSubsystem<UEconomySubsystem>();
  if (Economy) {
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