// SimPlayerController.cpp (ИСПРАВЛЕННАЯ ВЕРСИЯ)
// Здесь мы убираем все вызовы удаленных функций и переменных
// и переключаемся на работу с EconomySubsystem.

#include "SimPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EconomySubsystem.h" // <-- Важный инклюд для новой системы
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Logging/LogMacros.h"
#include "MoneyHUDWidget.h"
#include "SimSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogSimPC, Log, All);

ASimPlayerController::ASimPlayerController() {
  MappingPriority = 0;

  const USimSettings *GameSettings = GetDefault<USimSettings>();
  if (GameSettings) {
    MoneyHUDClass = GameSettings->MoneyHUDWidgetClass;
  }

  if (!MoneyHUDClass) {
    UE_LOG(LogSimPC, Warning,
           TEXT("[PC] MoneyHUDClass is not set in Project Settings!"));
  }

  static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCObj(
      TEXT("/Game/AssetInput/IMC_Default.IMC_Default"));
  if (IMCObj.Succeeded()) {
    DefaultMappingContext = IMCObj.Object;
  } else {
    UE_LOG(LogSimPC, Warning, TEXT("[PC] Failed to load Default IMC"));
  }
}

void ASimPlayerController::BeginPlay() {
  Super::BeginPlay();

  AddDefaultIMC();

  // Получаем подсистему экономики
  UEconomySubsystem *EconomySubsystem =
      GetGameInstance()->GetSubsystem<UEconomySubsystem>();
  if (!EconomySubsystem) {
    UE_LOG(LogSimPC, Error, TEXT("FATAL: EconomySubsystem not found!"));
    return;
  }

  if (MoneyHUDClass) {
    UMoneyHUDWidget *HUDWidget =
        CreateWidget<UMoneyHUDWidget>(this, MoneyHUDClass);
    if (HUDWidget) {
      HUDWidget->AddToViewport();
      MoneyHUD = HUDWidget;

      // Устанавливаем начальное значение из подсистемы и подписываемся на
      // будущие изменения
      HUDWidget->SetMoney(EconomySubsystem->GetCurrentBalance());
      EconomySubsystem->OnBalanceChanged.AddDynamic(HUDWidget,
                                                    &UMoneyHUDWidget::SetMoney);

      UE_LOG(LogSimPC, Log,
             TEXT("MoneyHUD created and bound to EconomySubsystem."));
    }
  }
}

// Функции LoadCurrentMoneyFromGameStateOrSave(), AddMoney() и OnPossess()
// ПОЛНОСТЬЮ УДАЛЕНЫ так как их больше не существует в .h файле.

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
    }
  }
}