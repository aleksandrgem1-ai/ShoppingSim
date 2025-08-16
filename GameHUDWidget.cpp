#include "GameHUDWidget.h"
#include "ClockWidget.h"
#include "DayInfoWidget.h"
#include "MoneyHUDWidget.h"

void UGameHUDWidget::NativeConstruct() {
  Super::NativeConstruct();
  // «десь можно добавить проверочные логи, если нужно
}

void UGameHUDWidget::SetMoney(int32 NewValue) {
  if (MoneyHUDWidget) {
    MoneyHUDWidget->SetMoney(NewValue);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[HUD] MoneyHUDWidget is nullptr"));
  }
}

void UGameHUDWidget::SetDayInfo(int32 Day, int32 Goal) {
  if (DayInfoWidget) {
    DayInfoWidget->SetDayInfo(Day, Goal);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[HUD] DayInfoWidget is nullptr"));
  }
}

void UGameHUDWidget::SetTime(int32 Hour, int32 Minute) {
  if (ClockWidget) {
    ClockWidget->SetTime(Hour, Minute);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[HUD] ClockWidget is nullptr"));
  }
}

void UGameHUDWidget::SetIncome(int32 NewIncome) {
  if (DayInfoWidget) {
    DayInfoWidget->SetIncome(NewIncome);
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[HUD] DayInfoWidget is nullptr (Income)"));
  }
}
