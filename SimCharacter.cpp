// SimCharacter.cpp
#include "SimCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "InteractableActor.h"

DEFINE_LOG_CATEGORY_STATIC(LogSimChar, Log, All);

ASimCharacter::ASimCharacter() {
  // Включаем Tick для работы трассировки луча в каждом кадре
  PrimaryActorTick.bCanEverTick = true;

  // Камера под контроллер игрока
  bUseControllerRotationYaw = true;
  bUseControllerRotationPitch = true;
  bUseControllerRotationRoll = false;

  if (UCharacterMovementComponent *MoveComp = GetCharacterMovement()) {
    MoveComp->bOrientRotationToMovement = false;
    MoveComp->RotationRate = FRotator(0.f, 540.f, 0.f);
    MoveComp->MaxWalkSpeed = 500.f;
  }

  FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
  FollowCamera->SetupAttachment(GetCapsuleComponent());
  FollowCamera->SetRelativeLocation(FVector(0.f, 0.f, 60.f)); // Центр "глаз"
  FollowCamera->bUsePawnControlRotation = true;

  // Загрузка IA ассетов
  static ConstructorHelpers::FObjectFinder<UInputAction> IA_MoveRef(
      TEXT("/Game/AssetInput/Asset/IA_Move.IA_Move"));
  static ConstructorHelpers::FObjectFinder<UInputAction> IA_LookRef(
      TEXT("/Game/AssetInput/Asset/IA_Look.IA_Look"));
  static ConstructorHelpers::FObjectFinder<UInputAction> IA_InteractRef(
      TEXT("/Game/AssetInput/Asset/IA_Interact.IA_Interact"));

  if (IA_MoveRef.Succeeded()) {
    MoveAction = IA_MoveRef.Object;
  } else {
    UE_LOG(LogSimChar, Error, TEXT("[Ctor] Failed to load IA_Move"));
  }

  if (IA_LookRef.Succeeded()) {
    LookAction = IA_LookRef.Object;
  } else {
    UE_LOG(LogSimChar, Error, TEXT("[Ctor] Failed to load IA_Look"));
  }

  if (IA_InteractRef.Succeeded()) {
    InteractAction = IA_InteractRef.Object;
  } else {
    UE_LOG(LogSimChar, Error, TEXT("[Ctor] Failed to load IA_Interact"));
  }
}

void ASimCharacter::BeginPlay() { Super::BeginPlay(); }

void ASimCharacter::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  if (UEnhancedInputComponent *EIC =
          Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
    if (MoveAction) {
      EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this,
                      &ASimCharacter::Move);
    }
    if (LookAction) {
      EIC->BindAction(LookAction, ETriggerEvent::Triggered, this,
                      &ASimCharacter::Look);
    }
    if (InteractAction) {
      EIC->BindAction(InteractAction, ETriggerEvent::Started, this,
                      &ASimCharacter::Interact);
    }
  }
}

void ASimCharacter::Move(const FInputActionValue &Value) {
  const FVector2D Input2D = Value.Get<FVector2D>();

  if (!Controller)
    return;

  const FRotator ControlRot(0.f, Controller->GetControlRotation().Yaw, 0.f);
  const FVector Forward = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::X);
  const FVector Right = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::Y);

  if (Input2D.SizeSquared() > KINDA_SMALL_NUMBER) {
    AddMovementInput(Forward, Input2D.Y);
    AddMovementInput(Right, Input2D.X);
  }
}

void ASimCharacter::Look(const FInputActionValue &Value) {
  const FVector2D Input = Value.Get<FVector2D>();
  AddControllerYawInput(Input.X);
  AddControllerPitchInput(Input.Y);
}

// --- ИСПРАВЛЕННАЯ ВЕРСИЯ ---
// Осталась только одна функция Interact с полной логикой.
void ASimCharacter::Interact(const FInputActionValue & /*Value*/) {
  UE_LOG(LogSimChar, Log, TEXT("[Interact] Input fired"));

  if (TargetedInteractable) {
    // Если мы смотрим на интерактивный объект, вызываем его функцию
    TargetedInteractable->OnInteract(GetController());
  }
}

void ASimCharacter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  // В каждом кадре проверяем, на что смотрит игрок
  TraceForInteractable();
}

void ASimCharacter::TraceForInteractable() {
  if (!Controller)
    return;

  FVector Location;
  FRotator Rotation;
  Controller->GetPlayerViewPoint(Location, Rotation);

  FVector End = Location + Rotation.Vector() * 300.f; // Луч длиной 3 метра

  FHitResult HitResult;
  FCollisionQueryParams Params;
  Params.AddIgnoredActor(this); // Игнорируем себя при трассировке

  bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Location, End,
                                                   ECC_Visibility, Params);

  // Включаем видимость луча для отладки
  DrawDebugLine(GetWorld(), Location, End, FColor::Green, false, 0.1f);

  if (bHit) {
    // Проверяем, является ли объект, в который попал луч, интерактивным
    AInteractableActor *Interactable =
        Cast<AInteractableActor>(HitResult.GetActor());

    if (Interactable) {
      // Если да, то сохраняем его как нашу цель
      TargetedInteractable = Interactable;
    } else {
      // Если нет, то сбрасываем цель
      TargetedInteractable = nullptr;
    }
  } else {
    // Если луч никуда не попал, тоже сбрасываем цель
    TargetedInteractable = nullptr;
  }
}