// SimPlayerController.cpp
#include "SimPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Logging/LogMacros.h"
#include "MoneyHUDWidget.h"
#include "SimSettings.h" // <-- Добавлен новый инклюд
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogSimPC, Log, All);

ASimPlayerController::ASimPlayerController() {
  MappingPriority = 0;

  // --- НАЧАЛО ИЗМЕНЕНИЙ ---

  // Получаем настройки проекта
  const USimSettings *GameSettings = GetDefault<USimSettings>();
  if (GameSettings) {
    // Берем класс виджета прямо из настроек
    MoneyHUDClass = GameSettings->MoneyHUDWidgetClass;
  }

  if (MoneyHUDClass) {
    UE_LOG(LogSimPC, Log,
           TEXT("[PC] MoneyHUDClass loaded from Project Settings: %s"),
           *MoneyHUDClass->GetName());
  } else {
    UE_LOG(LogSimPC, Warning,
           TEXT("[PC] MoneyHUDClass is not set in Project Settings! Please "
                "check Edit > Project Settings > Game > Sim Game Settings."));
  }

  // --- КОНЕЦ ИЗМЕНЕНИЙ ---

  static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCObj(
      TEXT("/Game/AssetInput/IMC_Default.IMC_Default"));
  if (IMCObj.Succeeded()) {
    DefaultMappingContext = IMCObj.Object;
    UE_LOG(LogSimPC, Log, TEXT("[PC] Default IMC loaded: %s"),
           *DefaultMappingContext->GetName());
  } else {
    UE_LOG(LogSimPC, Warning,
           TEXT("[PC] Failed to load Default IMC (check path)"));
  }
}

void ASimPlayerController::BeginPlay() {
  Super::BeginPlay();

  AddDefaultIMC();

  if (MoneyHUDClass) {
    UMoneyHUDWidget *HUDWidget =
        CreateWidget<UMoneyHUDWidget>(this, MoneyHUDClass);
    if (HUDWidget) {
      HUDWidget->AddToViewport();
      MoneyHUD = HUDWidget;

      UE_LOG(LogSimPC, Log,
             TEXT("[PC] MoneyHUD created and added to viewport"));
      UE_LOG(LogSimPC, Warning, TEXT("[PC] MoneyHUD created: %p, class=%s"),
             MoneyHUD, *GetNameSafe(MoneyHUDClass));

      // Восстанавливаем баланс из сохранения или состояния
      CurrentMoney = LoadCurrentMoneyFromGameStateOrSave();
      MoneyHUD->SetMoney(CurrentMoney);
      UE_LOG(LogSimPC, Log, TEXT("[PC] Money restored to: %d"), CurrentMoney);
    } else {
      UE_LOG(LogSimPC, Warning,
             TEXT("[PC] Failed to create MoneyHUD instance"));
    }
  } else {
    UE_LOG(LogSimPC, Warning,
           TEXT("[PC] MoneyHUDClass is null — assign in BP or defaults"));
  }
}

// ----------------------------
// Этот метод можно потом заменить на реальную логику загрузки
// ----------------------------
int32 ASimPlayerController::LoadCurrentMoneyFromGameStateOrSave() const {
  // Здесь ты можешь дернуть GameState, GameInstance или SaveGame
  // Сейчас — просто фиктивное значение для примера
  return 1500;
}

void ASimPlayerController::AddMoney(int32 Amount) {
  UE_LOG(LogSimPC, Warning, TEXT("[PC] AddMoney called with Amount=%d"),
         Amount);

  const int32 OldMoney = CurrentMoney;
  CurrentMoney += Amount;

  UE_LOG(LogSimPC, Warning, TEXT("[PC] Money changed: %d -> %d"), OldMoney,
         CurrentMoney);

  if (MoneyHUD) {
    UE_LOG(LogSimPC, Warning, TEXT("[PC] Updating HUD instance: %p"), MoneyHUD);
    MoneyHUD->SetMoney(CurrentMoney);
  } else {
    UE_LOG(LogSimPC, Warning,
           TEXT("[PC] MoneyHUD is null — HUD not created/assigned yet"));
  }
}

void ASimPlayerController::OnPossess(APawn *InPawn) {
  Super::OnPossess(InPawn);

  UE_LOG(LogSimPC, Log, TEXT("[PC] OnPossess: %s"), *GetNameSafe(InPawn));

  // При необходимости сразу обновляем HUD
  if (MoneyHUD) {
    MoneyHUD->SetMoney(CurrentMoney);
  }
}

void ASimPlayerController::AddDefaultIMC() {
  if (!DefaultMappingContext) {
    UE_LOG(LogSimPC, Warning, TEXT("[PC] DefaultMappingContext is null"));
    return;
  }

  if (ULocalPlayer *LocalPlayer = GetLocalPlayer()) {
    if (auto *InputSubsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                LocalPlayer)) {
      InputSubsystem->AddMappingContext(DefaultMappingContext, MappingPriority);
      UE_LOG(LogSimPC, Log, TEXT("[PC] Added Default IMC (priority %d)"),
             MappingPriority);
    } else {
      UE_LOG(LogSimPC, Warning, TEXT("[PC] EnhancedInput subsystem not found"));
    }
  }
}