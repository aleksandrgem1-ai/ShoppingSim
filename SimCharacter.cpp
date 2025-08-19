#include "Characters/SimCharacter.h"
#include "Actors/ComputerActor.h"
#include "Actors/InteractableActor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "Settings/SimSettings.h"
#include "Subsystems/BuildManagerSubsystem.h"
#include "UObject/ConstructorHelpers.h"
#include "Utils/AimTraceService.h"

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

  // Load input actions (from first file)
  static ConstructorHelpers::FObjectFinder<UInputAction> IA_MoveRef(
      TEXT("/Game/AssetInput/Asset/IA_Move.IA_Move"));
  if (IA_MoveRef.Succeeded())
    MoveAction = IA_MoveRef.Object;

  static ConstructorHelpers::FObjectFinder<UInputAction> IA_LookRef(
      TEXT("/Game/AssetInput/Asset/IA_Look.IA_Look"));
  if (IA_LookRef.Succeeded())
    LookAction = IA_LookRef.Object;

  static ConstructorHelpers::FObjectFinder<UInputAction> IA_InteractRef(
      TEXT("/Game/AssetInput/Asset/IA_Interact.IA_Interact"));
  if (IA_InteractRef.Succeeded())
    InteractAction = IA_InteractRef.Object;

  static ConstructorHelpers::FObjectFinder<UInputAction> IA_ToggleBuildRef(
      TEXT("/Game/AssetInput/Asset/IA_ToggleBuildMode.IA_ToggleBuildMode"));
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

void ASimCharacter::Interact(const FInputActionValue & /*Value*/) {
  if (!TargetedInteractable)
    return;

  // Если это компьютер (AComputerActor), включить/выключить UI и сменить режим
  // ввода
  if (AComputerActor *Computer =
          Cast<AComputerActor>(TargetedInteractable.Get())) {
    if (Computer->DesktopWidgetComp) {
      bool bIsVisible = Computer->DesktopWidgetComp->IsVisible();

      Computer->DesktopWidgetComp->SetVisibility(!bIsVisible);

      APlayerController *PC = Cast<APlayerController>(GetController());
      if (PC) {
        if (!bIsVisible) {
          PC->bShowMouseCursor = true;
          PC->SetInputMode(FInputModeUIOnly());
        } else {
          PC->bShowMouseCursor = false;
          PC->SetInputMode(FInputModeGameOnly());
        }
      }
      return;
    }
  }

  // Иначе, если это любой другой Interactable (ASimCharacter first file)
  if (AInteractableActor *Interactable =
          Cast<AInteractableActor>(TargetedInteractable.Get())) {
    Interactable->OnInteract(GetController());
  }
}

void ASimCharacter::ToggleBuildMode() {
  if (UBuildManagerSubsystem *BuildManager =
          GetGameInstance()->GetSubsystem<UBuildManagerSubsystem>()) {
    BuildManager->ToggleBuildMode();
  }
}

void ASimCharacter::TraceForInteractable() {
  if (!Controller)
    return;

  const USimSettings *Settings = GetDefault<USimSettings>();
  const float Distance = Settings ? Settings->TraceDistanceInteract : 15000.f;
  const ECollisionChannel Channel =
      Settings ? Settings->TraceChannelInteract.GetValue() : ECC_Visibility;

  TArray<const AActor *> Ignore;
  Ignore.Add(this);

  FHitResult HitResult;

  const bool bHit = FAimTraceService::TraceFromScreenCenter(
      GetWorld(), Cast<APlayerController>(Controller), Distance, Channel,
      Ignore, HitResult, false);

  if (bHit) {
    TargetedInteractable = HitResult.GetActor();
  } else {
    TargetedInteractable = nullptr;
  }
}
