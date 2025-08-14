// SimCharacter.cpp (ПОЛНАЯ ВЕРСИЯ)

#include "SimCharacter.h"
#include "BuildManagerSubsystem.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "InteractableActor.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogSimChar, Log, All);

ASimCharacter::ASimCharacter() {
  PrimaryActorTick.bCanEverTick = true;

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
  FollowCamera->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
  FollowCamera->bUsePawnControlRotation = true;

  static ConstructorHelpers::FObjectFinder<UInputAction> IA_MoveRef(
      TEXT("/Game/AssetInput/Asset/IA_Move"));
  if (IA_MoveRef.Succeeded())
    MoveAction = IA_MoveRef.Object;

  static ConstructorHelpers::FObjectFinder<UInputAction> IA_LookRef(
      TEXT("/Game/AssetInput/Asset/IA_Look"));
  if (IA_LookRef.Succeeded())
    LookAction = IA_LookRef.Object;

  static ConstructorHelpers::FObjectFinder<UInputAction> IA_InteractRef(
      TEXT("/Game/AssetInput/Asset/IA_Interact"));
  if (IA_InteractRef.Succeeded())
    InteractAction = IA_InteractRef.Object;

  static ConstructorHelpers::FObjectFinder<UInputAction> IA_ToggleBuildRef(
      TEXT("/Game/AssetInput/Asset/IA_ToggleBuildMode"));
  if (IA_ToggleBuildRef.Succeeded())
    ToggleBuildModeAction = IA_ToggleBuildRef.Object;
}

void ASimCharacter::BeginPlay() { Super::BeginPlay(); }

void ASimCharacter::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  if (UEnhancedInputComponent *EIC =
          Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
    if (MoveAction)
      EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this,
                      &ASimCharacter::Move);
    if (LookAction)
      EIC->BindAction(LookAction, ETriggerEvent::Triggered, this,
                      &ASimCharacter::Look);
    if (InteractAction)
      EIC->BindAction(InteractAction, ETriggerEvent::Started, this,
                      &ASimCharacter::Interact);

    if (ToggleBuildModeAction)
      EIC->BindAction(ToggleBuildModeAction, ETriggerEvent::Started, this,
                      &ASimCharacter::ToggleBuildMode);
  }
}

void ASimCharacter::ToggleBuildMode() {
  if (UBuildManagerSubsystem *BuildManager =
          GetGameInstance()->GetSubsystem<UBuildManagerSubsystem>()) {
    BuildManager->ToggleBuildMode();
  }
}

void ASimCharacter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
  TraceForInteractable();
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

void ASimCharacter::Interact(const FInputActionValue &Value) {
  if (TargetedInteractable) {
    TargetedInteractable->OnInteract(GetController());
  }
}

void ASimCharacter::TraceForInteractable() {
  if (!Controller)
    return;
  FVector Location;
  FRotator Rotation;
  Controller->GetPlayerViewPoint(Location, Rotation);
  FVector End = Location + Rotation.Vector() * 300.f;
  FHitResult HitResult;
  FCollisionQueryParams Params;
  Params.AddIgnoredActor(this);
  bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Location, End,
                                                   ECC_Visibility, Params);
  if (bHit) {
    TargetedInteractable = Cast<AInteractableActor>(HitResult.GetActor());
  } else {
    TargetedInteractable = nullptr;
  }
}