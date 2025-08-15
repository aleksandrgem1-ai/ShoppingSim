// ShoppingSim/DayInfoWidget.h

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "DayInfoWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOPPINGSIM_API UDayInfoWidget : public UUserWidget {
  GENERATED_BODY()

protected:
  virtual void NativeConstruct() override;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> DayText;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> IncomeText;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> GoalText;

  // днаюбкъел UFUNCTION()
  UFUNCTION()
  void UpdateDayInfo(int32 NewDay, int32 PreviousDayIncome,
                     int32 PreviousDayGoal, int32 NewDayGoal);
};