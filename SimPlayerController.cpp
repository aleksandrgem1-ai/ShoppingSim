// SimPlayerController.cpp

#include "Controllers/SimPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Settings/SimSettings.h"
#include "Subsystems/EconomySubsystem.h"
#include "Subsystems/EventManagerSubsystem.h"
#include "Subsystems/TimeManagerSubsystem.h"
#include "UI/HUD/GameHUDWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogSimPC, Log, All);

ASimPlayerController::ASimPlayerController() {
  MappingPriority = 0;

  const USimSettings *GameSettings = GetDefault<USimSettings>();
  if (GameSettings) {
    GameHUDClass = GameSettings->GameHUDWidgetClass;
  }

  static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCObj(
      TEXT("/Game/AssetInput/IMC_Default.IMC_Default"));
  if (IMCObj.Succeeded()) {
    DefaultMappingContext = IMCObj.Object;
  }
}

void ASimPlayerController::BeginPlay() {
  Super::BeginPlay();

  AddDefaultIMC();

  // Подписка на события
  if (UEventManagerSubsystem *EMS =
          GetWorld()->GetSubsystem<UEventManagerSubsystem>()) {
    EMS->OnGameEvent.AddDynamic(this, &ASimPlayerController::HandleGameEvent);
    UE_LOG(LogSimPC, Warning, TEXT("[PC] Subscribed to EMS.OnGameEvent"));
  } else {
    UE_LOG(LogSimPC, Error, TEXT("[PC] EMS is missing in this world"));
  }

  if (GameHUDClass) {
    GameHUD = CreateWidget<UGameHUDWidget>(this, GameHUDClass);
    if (GameHUD) {
      GameHUD->AddToViewport();

      if (UEconomySubsystem *Economy =
              GetGameInstance()->GetSubsystem<UEconomySubsystem>()) {
        GameHUD->SetMoney(Economy->GetCurrentBalance());
        Economy->OnBalanceChanged.AddDynamic(GameHUD,
                                             &UGameHUDWidget::SetMoney);
        GameHUD->SetIncome(Economy->GetCurrentIncome());
        Economy->OnIncomeChanged.AddDynamic(GameHUD,
                                            &UGameHUDWidget::SetIncome);
      }

      if (UTimeManagerSubsystem *TimeManager =
              GetWorld()->GetSubsystem<UTimeManagerSubsystem>()) {
        int32 Day, Goal;
        TimeManager->GetCurrentDayInfo(Day, Goal);
        GameHUD->SetDayInfo(Day, Goal);
        GameHUD->SetTime(0, 0);
        TimeManager->OnTimeUpdated.AddDynamic(GameHUD,
                                              &UGameHUDWidget::SetTime);
      }
    }
  }
}

void ASimPlayerController::AddDefaultIMC() {
  if (!DefaultMappingContext)
    return;

  if (ULocalPlayer *LocalPlayer = GetLocalPlayer()) {
    if (auto *InputSubsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                LocalPlayer)) {
      InputSubsystem->AddMappingContext(DefaultMappingContext, /*Priority*/ 0);
    }
  }
}

void ASimPlayerController::HandleGameEvent(const FText &EventText) {
  UE_LOG(LogSimPC, Warning, TEXT("[PC] GameEvent: %s"), *EventText.ToString());
}
