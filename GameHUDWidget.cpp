// GameHUDWidget.cpp (ОБНОВЛЕННЫЙ)

#include "GameHUDWidget.h"
#include "ClockWidget.h"
#include "DayInfoWidget.h"
#include "MoneyHUDWidget.h" // <-- Инклюдим ваш класс

void UGameHUDWidget::SetMoney(int32 NewValue) {
  if (MoneyWidgetInstance) {
    MoneyWidgetInstance->SetMoney(NewValue);
  }
}

void UGameHUDWidget::SetTime(int32 Hour, int32 Minute) {
  if (ClockWidgetInstance) {
    ClockWidgetInstance->SetTime(Hour, Minute);
  }
}

void UGameHUDWidget::SetDayInfo(int32 Day, int32 Goal) {
  if (DayInfoWidgetInstance) {
    DayInfoWidgetInstance->SetDayInfo(Day, Goal);
  }
}