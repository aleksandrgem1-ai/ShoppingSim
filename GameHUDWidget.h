// ShoppingSim/GameHUDWidget.h

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameHUDWidget.generated.h"

class UClockWidget;
class UDayInfoWidget;
class UMoneyHUDWidget;

UCLASS()
class SHOPPINGSIM_API UGameHUDWidget : public UUserWidget {
  GENERATED_BODY()

protected:
  // --- днаюбкъел меднярючыее назъбкемхе ---
  virtual void NativeConstruct() override;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UClockWidget> ClockWidget;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UDayInfoWidget> DayInfoWidget;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UMoneyHUDWidget> MoneyHUDWidget;
};
