// GameHUDWidget.cpp
#include "UI/HUD/GameHUDWidget.h"
#include "Subsystems/EconomySubsystem.h"
#include "Subsystems/TimeManagerSubsystem.h"
#include "UI/HUD/ClockWidget.h"
#include "UI/HUD/DayInfoWidget.h"
#include "UI/HUD/MoneyHUDWidget.h"

void UGameHUDWidget::NativeConstruct() { Super::NativeConstruct(); }

void UGameHUDWidget::NativeDestruct() {
  if (UWorld *World = GetWorld()) {
    if (UEconomySubsystem *Economy =
            World->GetGameInstance()->GetSubsystem<UEconomySubsystem>()) {
      Economy->OnBalanceChanged.RemoveDynamic(this, &UGameHUDWidget::SetMoney);
      Economy->OnIncomeChanged.RemoveDynamic(this, &UGameHUDWidget::SetIncome);
    }

    if (UTimeManagerSubsystem *TimeManager =
            World->GetSubsystem<UTimeManagerSubsystem>()) {
      TimeManager->OnTimeUpdated.RemoveDynamic(this, &UGameHUDWidget::SetTime);
    }
  }

  Super::NativeDestruct();
}

void UGameHUDWidget::SetMoney(int32 NewValue) {
  if (MoneyHUDWidget) {
    MoneyHUDWidget->SetMoney(NewValue);
  }
}

void UGameHUDWidget::SetDayInfo(int32 Day, int32 Goal) {
  if (DayInfoWidget) {
    DayInfoWidget->SetDayInfo(Day, Goal);
  }
}

void UGameHUDWidget::SetTime(int32 Hour, int32 Minute) {
  if (ClockWidget) {
    ClockWidget->SetTime(Hour, Minute);
  }
}

void UGameHUDWidget::SetIncome(int32 NewIncome) {
  if (DayInfoWidget) {
    DayInfoWidget->SetIncome(NewIncome);
  }
}
