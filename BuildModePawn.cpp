// BuildModePawn.cpp (ÏÎËÍÀß ÂÅÐÑÈß)

#include "BuildModePawn.h"
#include "BuildManagerSubsystem.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "InputAction.h"
#include "StoreZoneActor.h"
#include "UObject/ConstructorHelpers.h"

ABuildModePawn::ABuildModePawn() {
  PrimaryActorTick.bCanEverTick = true;

  USceneComponent *Root =
      CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  RootComponent = Root;

  CameraComponent =
      CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
  CameraComponent->SetupAttachment(RootComponent);
  CameraComponent->bUsePawnControlRotation = true;

  MovementComponent =
      CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
  MovementComponent->UpdatedComponent = RootComponent;

  static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(
      TEXT("/Game/AssetInput/Asset/IA_Move"));
  if (MoveActionRef.Succeeded())
    MoveAction = MoveActionRef.Object;

  static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(
      TEXT("/Game/AssetInput/Asset/IA_Look"));
  if (LookActionRef.Succeeded())
    LookAction = LookActionRef.Object;

  static ConstructorHelpers::FObjectFinder<UInputAction> FlyUpActionRef(
      TEXT("/Game/AssetInput/Asset/IA_Build_FlyUp"));
  if (FlyUpActionRef.Succeeded())
    FlyUpAction = FlyUpActionRef.Object;

  static ConstructorHelpers::FObjectFinder<UInputAction> FlyDownActionRef(
      TEXT("/Game/AssetInput/Asset/IA_Build_FlyDown"));
  if (FlyDownActionRef.Succeeded())
    FlyDownAction = FlyDownActionRef.Object;

  static ConstructorHelpers::FObjectFinder<UInputAction> PlaceActionRef(
      TEXT("/Game/AssetInput/Asset/IA_Place"));
  if (PlaceActionRef.Succeeded())
    PlaceAction = PlaceActionRef.Object;

  static ConstructorHelpers::FObjectFinder<UInputAction> RemoveActionRef(
      TEXT("/Game/AssetInput/Asset/IA_RemoveZone"));
  if (RemoveActionRef.Succeeded())
    RemoveAction = RemoveActionRef.Object;

  static ConstructorHelpers::FObjectFinder<UInputAction> DestroyActionRef(
      TEXT("/Game/AssetInput/Asset/IA_Destroy"));
  if (DestroyActionRef.Succeeded())
    DestroyAction = DestroyActionRef.Object;

  static ConstructorHelpers::FObjectFinder<UInputAction> RotateActionRef(
      TEXT("/Game/AssetInput/Asset/IA_Rotate"));
  if (RotateActionRef.Succeeded())
    RotateAction = RotateActionRef.Object;

  static ConstructorHelpers::FObjectFinder<UInputAction> ToggleGridActionRef(
      TEXT("/Game/AssetInput/Asset/IA_ToggleGrid"));
  if (ToggleGridActionRef.Succeeded())
    ToggleGridAction = ToggleGridActionRef.Object;

  static ConstructorHelpers::FObjectFinder<UInputAction> ExitBuildModeActionRef(
      TEXT("/Game/AssetInput/Asset/IA_ExitBuildMode"));
  if (ExitBuildModeActionRef.Succeeded())
    ExitBuildModeAction = ExitBuildModeActionRef.Object;

  static ConstructorHelpers::FObjectFinder<UInputAction>
      ToggleBuildModeActionRef(
          TEXT("/Game/AssetInput/Asset/IA_ToggleBuildMode"));
  if (ToggleBuildModeActionRef.Succeeded())
    ToggleBuildModeAction = ToggleBuildModeActionRef.Object;
}

void ABuildModePawn::BeginPlay() {
  Super::BeginPlay();
  if (UBuildManagerSubsystem *BuildManager =
          GetGameInstance()->GetSubsystem<UBuildManagerSubsystem>()) {
    BuildManager->StartPlacement();
  }
}

void ABuildModePawn::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  if (UBuildManagerSubsystem *BuildManager =
          GetGameInstance()->GetSubsystem<UBuildManagerSubsystem>()) {
    BuildManager->UpdateGhostActorTransform();
  }
}

void ABuildModePawn::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);
  if (UEnhancedInputComponent *EIC =
          Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
    if (MoveAction)
      EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this,
                      &ABuildModePawn::Move);
    if (LookAction)
      EIC->BindAction(LookAction, ETriggerEvent::Triggered, this,
                      &ABuildModePawn::Look);
    if (FlyUpAction)
      EIC->BindAction(FlyUpAction, ETriggerEvent::Triggered, this,
                      &ABuildModePawn::HandleFlyUp);
    if (FlyDownAction)
      EIC->BindAction(FlyDownAction, ETriggerEvent::Triggered, this,
                      &ABuildModePawn::HandleFlyDown);
    if (PlaceAction)
      EIC->BindAction(PlaceAction, ETriggerEvent::Started, this,
                      &ABuildModePawn::HandlePlace);
    if (RemoveAction)
      EIC->BindAction(RemoveAction, ETriggerEvent::Started, this,
                      &ABuildModePawn::HandleRemove);
    if (DestroyAction)
      EIC->BindAction(DestroyAction, ETriggerEvent::Started, this,
                      &ABuildModePawn::HandleDestroy);
    if (RotateAction)
      EIC->BindAction(RotateAction, ETriggerEvent::Triggered, this,
                      &ABuildModePawn::HandleRotate);
    if (ToggleGridAction)
      EIC->BindAction(ToggleGridAction, ETriggerEvent::Started, this,
                      &ABuildModePawn::HandleToggleGrid);

    if (ExitBuildModeAction)
      EIC->BindAction(ExitBuildModeAction, ETriggerEvent::Started, this,
                      &ABuildModePawn::HandleExitBuildMode);
    if (ToggleBuildModeAction)
      EIC->BindAction(ToggleBuildModeAction, ETriggerEvent::Started, this,
                      &ABuildModePawn::HandleExitBuildMode);
  }
}

void ABuildModePawn::Move(const FInputActionValue &Value) {
  const FVector2D Input2D = Value.Get<FVector2D>();
  if (Controller && (Input2D.X != 0.f || Input2D.Y != 0.f)) {
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    const FVector ForwardDirection =
        FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection =
        FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDirection, Input2D.Y);
    AddMovementInput(RightDirection, Input2D.X);
  }
}

void ABuildModePawn::Look(const FInputActionValue &Value) {
  const FVector2D LookAxisVector = Value.Get<FVector2D>();
  if (Controller) {
    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
  }
}

void ABuildModePawn::HandleFlyUp(const FInputActionValue &Value) {
  AddMovementInput(FVector::UpVector, 1.0f);
}

void ABuildModePawn::HandleFlyDown(const FInputActionValue &Value) {
  AddMovementInput(FVector::DownVector, 1.0f);
}

void ABuildModePawn::HandlePlace() {
  if (UBuildManagerSubsystem *BuildManager =
          GetGameInstance()->GetSubsystem<UBuildManagerSubsystem>()) {
    BuildManager->ConfirmPlacement();
  }
}

void ABuildModePawn::HandleRemove() {
  APlayerController *PC = GetController<APlayerController>();
  if (!PC)
    return;
  FVector Start, Dir;
  PC->DeprojectMousePositionToWorld(Start, Dir);
  FVector End = Start + (Dir * 10000.f);
  FHitResult HitResult;
  GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
  if (HitResult.bBlockingHit) {
    if (AStoreZoneActor *ZoneToInteract =
            Cast<AStoreZoneActor>(HitResult.GetActor())) {
      if (UBuildManagerSubsystem *BuildManager =
              GetGameInstance()->GetSubsystem<UBuildManagerSubsystem>()) {
        BuildManager->SelectObjectForMove(ZoneToInteract);
      }
    }
  }
}

void ABuildModePawn::HandleDestroy() {
  APlayerController *PC = GetController<APlayerController>();
  if (!PC)
    return;
  FVector Start, Dir;
  PC->DeprojectMousePositionToWorld(Start, Dir);
  FVector End = Start + (Dir * 10000.f);
  FHitResult HitResult;
  GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
  if (HitResult.bBlockingHit) {
    if (AStoreZoneActor *ZoneToDestroy =
            Cast<AStoreZoneActor>(HitResult.GetActor())) {
      if (UBuildManagerSubsystem *BuildManager =
              GetGameInstance()->GetSubsystem<UBuildManagerSubsystem>()) {
        BuildManager->HandleDestruction(ZoneToDestroy);
      }
    }
  }
}

void ABuildModePawn::HandleRotate(const FInputActionValue &Value) {
  const float RotationValue = Value.Get<float>();
  if (UBuildManagerSubsystem *BuildManager =
          GetGameInstance()->GetSubsystem<UBuildManagerSubsystem>()) {
    BuildManager->AddRotation(RotationValue * 45.0f);
  }
}

void ABuildModePawn::HandleToggleGrid() {
  if (UBuildManagerSubsystem *BuildManager =
          GetGameInstance()->GetSubsystem<UBuildManagerSubsystem>()) {
    BuildManager->ToggleGridSnapping();
  }
}

void ABuildModePawn::HandleExitBuildMode() {
  if (UBuildManagerSubsystem *BuildManager =
          GetGameInstance()->GetSubsystem<UBuildManagerSubsystem>()) {
    BuildManager->ToggleBuildMode();
  }
}