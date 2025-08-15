// GameHUDWidget.h (ОБНОВЛЕННЫЙ)

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameHUDWidget.generated.h"

class UMoneyHUDWidget; // <-- Используем ваш класс
class UClockWidget;
class UDayInfoWidget;

UCLASS()
class SHOPPINGSIM_API UGameHUDWidget : public UUserWidget {
  GENERATED_BODY()

public:
  void SetMoney(int32 NewValue);
  void SetTime(int32 Hour, int32 Minute);
  void SetDayInfo(int32 Day, int32 Goal);

protected:
  // --- Привязки к виджетам в Blueprint ---

  // Привязка к вашему виджету денег
  // (имя в BP должно быть MoneyWidgetInstance)
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UMoneyHUDWidget> MoneyWidgetInstance;

  // Привязка к виджету часов
  // (имя в BP должно быть ClockWidgetInstance)
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UClockWidget> ClockWidgetInstance;

  // Привязка к виджету информации о дне
  // (имя в BP должно быть DayInfoWidgetInstance)
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UDayInfoWidget> DayInfoWidgetInstance;
};