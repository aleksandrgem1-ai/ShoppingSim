// BuildManagerSubsystem.cpp (�����������)

#include "BuildManagerSubsystem.h"
#include "BuildModePawn.h" // <-- ��������� ������ ������ Pawn
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h" // <-- ��������� ������ ��� IMC
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h" // <-- ������ ������

void UBuildManagerSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
  Super::Initialize(Collection);

  // --- �������� ������� � C++ ---
  // ���������, ��� ���� ������������� ������ �������!
  static ConstructorHelpers::FObjectFinder<UInputMappingContext>
      PlayerContextRef(TEXT("/Game/AssetInput/IMC_Default"));
  if (PlayerContextRef.Succeeded()) {
    PlayerMappingContext = PlayerContextRef.Object;
  }

  static ConstructorHelpers::FObjectFinder<UInputMappingContext>
      BuildContextRef(TEXT("/Game/AssetInput/IMC_BuildMode"));
  if (BuildContextRef.Succeeded()) {
    BuildModeMappingContext = BuildContextRef.Object;
  }

  // ��������� ��� C++ ����� ��������
  BuildModePawnClass = ABuildModePawn::StaticClass();
}

void UBuildManagerSubsystem::EnterBuildMode() {
  APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (!PC || !BuildModePawnClass)
    return;

  // 1. ��������� ������������� ��������� � ��������� ���
  OriginalPlayerPawn = PC->GetPawn();
  if (OriginalPlayerPawn) {
    OriginalPlayerPawn->DisableInput(PC);
  }

  // 2. ������� � ��������� � ������ ��� �������������
  FActorSpawnParameters SpawnParams;
  SpawnParams.Owner = PC;
  APawn *BuildPawn = GetWorld()->SpawnActor<APawn>(
      BuildModePawnClass, OriginalPlayerPawn->GetActorLocation(),
      OriginalPlayerPawn->GetActorRotation(), SpawnParams);
  PC->Possess(BuildPawn);

  // 3. ������ �������� ����� (IMC)
  if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
          ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
              PC->GetLocalPlayer())) {
    Subsystem->RemoveMappingContext(PlayerMappingContext);
    Subsystem->AddMappingContext(BuildModeMappingContext, 0);
  }

  UE_LOG(LogTemp, Warning, TEXT("Entering Build Mode..."));
}

void UBuildManagerSubsystem::ExitBuildMode() {
  APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (!PC)
    return;

  // 1. ���������� ������ �������������
  if (APawn *CurrentPawn = PC->GetPawn()) {
    CurrentPawn->Destroy();
  }

  // 2. ���������� ���������� ������������� ���������
  if (OriginalPlayerPawn) {
    PC->Possess(OriginalPlayerPawn);
    OriginalPlayerPawn->EnableInput(PC);
  }

  // 3. ���������� ������ IMC
  if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
          ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
              PC->GetLocalPlayer())) {
    Subsystem->RemoveMappingContext(BuildModeMappingContext);
    Subsystem->AddMappingContext(PlayerMappingContext, 0);
  }

  UE_LOG(LogTemp, Warning, TEXT("Exiting Build Mode..."));
}