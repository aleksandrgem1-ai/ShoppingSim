#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "MoneyHUDWidget.generated.h"

/**
 * HUD‑виджет для отображения текущего баланса игрока
 */
UCLASS()
class SHOPPINGSIM_API UMoneyHUDWidget : public UUserWidget {
  GENERATED_BODY()

public:
  /** Обновляет отображаемую сумму */
  UFUNCTION(BlueprintCallable)
  void SetMoney(int32 NewValue);

protected:
  /** Привязка к TextBlock в BP (имя должно быть EXACT MoneyText) */
  UPROPERTY(meta = (BindWidget))
  class UTextBlock *MoneyText = nullptr;
};
