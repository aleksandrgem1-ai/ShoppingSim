// SimPlayerController.cpp (¬¿ÿ ‘¿…À, ¡≈« »«Ã≈Õ≈Õ»…)

#include "SimPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EconomySubsystem.h"
#include "EnhancedInputSubsystems.h"
#include "GameHUDWidget.h"
#include "InputMappingContext.h"
#include "SimSettings.h"
#include "TimeManagerSubsystem.h"

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
      InputSubsystem->AddMappingContext(DefaultMappingContext, MappingPriority);
    }
  }
}