// DayInfoWidget.h (НОВЫЙ ФАЙЛ)

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "DayInfoWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOPPINGSIM_API UDayInfoWidget : public UUserWidget {
  GENERATED_BODY()

public:
  // Обновляет текст с информацией о дне
  UFUNCTION(BlueprintCallable)
  void SetDayInfo(int32 Day, int32 Goal);

protected:
  // Привязка к TextBlock для дня (имя в BP должно быть DayText)
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> DayText;
};