// BuildManagerSubsystem.cpp (ОБНОВЛЕННЫЙ)

#include "BuildManagerSubsystem.h"
#include "BuildModePawn.h" // <-- Добавляем инклюд нашего Pawn
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h" // <-- Добавляем инклюд для IMC
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h" // <-- Важный инклюд

void UBuildManagerSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
  Super::Initialize(Collection);

  // --- ЗАГРУЗКА АССЕТОВ В C++ ---
  // Убедитесь, что пути соответствуют вашему проекту!
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

  // Назначаем наш C++ класс напрямую
  BuildModePawnClass = ABuildModePawn::StaticClass();
}

void UBuildManagerSubsystem::EnterBuildMode() {
  APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (!PC || !BuildModePawnClass)
    return;

  // 1. Сохраняем оригинального персонажа и отключаем его
  OriginalPlayerPawn = PC->GetPawn();
  if (OriginalPlayerPawn) {
    OriginalPlayerPawn->DisableInput(PC);
  }

  // 2. Создаем и вселяемся в камеру для строительства
  FActorSpawnParameters SpawnParams;
  SpawnParams.Owner = PC;
  APawn *BuildPawn = GetWorld()->SpawnActor<APawn>(
      BuildModePawnClass, OriginalPlayerPawn->GetActorLocation(),
      OriginalPlayerPawn->GetActorRotation(), SpawnParams);
  PC->Possess(BuildPawn);

  // 3. Меняем контекст ввода (IMC)
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

  // 1. Уничтожаем камеру строительства
  if (APawn *CurrentPawn = PC->GetPawn()) {
    CurrentPawn->Destroy();
  }

  // 2. Возвращаем управление оригинальному персонажу
  if (OriginalPlayerPawn) {
    PC->Possess(OriginalPlayerPawn);
    OriginalPlayerPawn->EnableInput(PC);
  }

  // 3. Возвращаем старый IMC
  if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
          ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
              PC->GetLocalPlayer())) {
    Subsystem->RemoveMappingContext(BuildModeMappingContext);
    Subsystem->AddMappingContext(PlayerMappingContext, 0);
  }

  UE_LOG(LogTemp, Warning, TEXT("Exiting Build Mode..."));
}