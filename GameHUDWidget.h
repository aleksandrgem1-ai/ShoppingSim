// GameHUDWidget.h
#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameHUDWidget.generated.h"

class UMoneyHUDWidget;
class UClockWidget;
class UDayInfoWidget;

UCLASS()
class SHOPPINGSIM_API UGameHUDWidget : public UUserWidget {
  GENERATED_BODY()

public:
  UFUNCTION() void SetMoney(int32 NewValue);
  UFUNCTION() void SetDayInfo(int32 Day, int32 Goal);
  UFUNCTION() void SetTime(int32 Hour, int32 Minute);
  UFUNCTION() void SetIncome(int32 NewIncome);

protected:
  virtual void NativeConstruct() override;
  virtual void NativeDestruct() override;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UClockWidget> ClockWidget;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UDayInfoWidget> DayInfoWidget;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UMoneyHUDWidget> MoneyHUDWidget;
};
