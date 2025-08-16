#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "DayInfoWidget.generated.h"

class UTextBlock;

/**
 * Виджет отображения информации о дне, цели и текущем доходе
 */
UCLASS()
class SHOPPINGSIM_API UDayInfoWidget : public UUserWidget {
  GENERATED_BODY()

public:
  /** Устанавливает значения дня и цели */
  UFUNCTION(BlueprintCallable)
  void SetDayInfo(int32 Day, int32 Goal);

  /** Устанавливает текущее значение дохода */
  UFUNCTION(BlueprintCallable)
  void SetIncome(int32 CurrentIncome);

protected:
  virtual void NativeConstruct() override;

  /** Текст с номером дня */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> DayText;

  /** Текст с текущим доходом */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> IncomeText;

  /** Текст с целью */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> GoalText;

  /** Обновление при смене дня из TimeManagerSubsystem */
  UFUNCTION()
  void UpdateDayInfo(int32 NewDay, int32 PreviousDayIncome,
                     int32 PreviousDayGoal, int32 NewDayGoal);
};
