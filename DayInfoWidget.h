#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "DayInfoWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOPPINGSIM_API UDayInfoWidget : public UUserWidget {
  GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable)
  void SetDayInfo(int32 Day, int32 Goal);

  UFUNCTION(BlueprintCallable)
  void SetIncome(int32 CurrentIncome);

protected:
  virtual void NativeConstruct() override;
  virtual void NativeDestruct() override;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> DayText;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> IncomeText;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> GoalText;

  UFUNCTION()
  void UpdateDayInfo(int32 NewDay, int32 PreviousDayIncome,
                     int32 PreviousDayGoal, int32 NewDayGoal);
};
