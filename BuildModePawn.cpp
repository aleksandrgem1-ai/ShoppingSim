// BuildModePawn.cpp (ОБНОВЛЕННЫЙ)

#include "BuildModePawn.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "InputAction.h"
#include "UObject/ConstructorHelpers.h" // <-- Важный инклюд для загрузки ассетов

ABuildModePawn::ABuildModePawn() {
  PrimaryActorTick.bCanEverTick = false;

  // Создаем компоненты
  USceneComponent *Root =
      CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  RootComponent = Root;

  CameraComponent =
      CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
  CameraComponent->SetupAttachment(RootComponent);
  CameraComponent->bUsePawnControlRotation =
      true; // <-- Важно для управления мышью

  MovementComponent =
      CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
  MovementComponent->UpdatedComponent = RootComponent;

  // --- ЗАГРУЗКА АССЕТОВ ВВОДА В C++ ---
  // Убедитесь, что пути соответствуют вашему проекту!
  static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(
      TEXT("/Game/AssetInput/Asset/IA_Move"));
  if (MoveActionRef.Succeeded()) {
    MoveAction = MoveActionRef.Object;
  }

  static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(
      TEXT("/Game/AssetInput/Asset/IA_Look"));
  if (LookActionRef.Succeeded()) {
    LookAction = LookActionRef.Object;
  }

  static ConstructorHelpers::FObjectFinder<UInputAction> FlyUpActionRef(
      TEXT("/Game/AssetInput/Asset/IA_Build_FlyUp"));
  if (FlyUpActionRef.Succeeded()) {
    FlyUpAction = FlyUpActionRef.Object;
  }

  static ConstructorHelpers::FObjectFinder<UInputAction> FlyDownActionRef(
      TEXT("/Game/AssetInput/Asset/IA_Build_FlyDown"));
  if (FlyDownActionRef.Succeeded()) {
    FlyDownAction = FlyDownActionRef.Object;
  }
}

void ABuildModePawn::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  if (UEnhancedInputComponent *EIC =
          Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
    // Привязываем действия к функциям
    if (MoveAction) {
      EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this,
                      &ABuildModePawn::Move);
    }
    if (LookAction) {
      EIC->BindAction(LookAction, ETriggerEvent::Triggered, this,
                      &ABuildModePawn::Look);
    }
    if (FlyUpAction) {
      EIC->BindAction(FlyUpAction, ETriggerEvent::Triggered, this,
                      &ABuildModePawn::HandleFlyUp);
    }
    if (FlyDownAction) {
      EIC->BindAction(FlyDownAction, ETriggerEvent::Triggered, this,
                      &ABuildModePawn::HandleFlyDown);
    }
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
  // Движение вверх по оси Z
  AddMovementInput(FVector::UpVector, Value.Get<float>());
}

void ABuildModePawn::HandleFlyDown(const FInputActionValue &Value) {
  // Движение вниз по оси Z
  AddMovementInput(FVector::DownVector, Value.Get<float>());
}