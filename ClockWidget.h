// ClockWidget.h (НОВЫЙ ФАЙЛ)

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "ClockWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOPPINGSIM_API UClockWidget : public UUserWidget {
  GENERATED_BODY()

public:
  // Обновляет текст с временем
  UFUNCTION(BlueprintCallable)
  void SetTime(int32 Hour, int32 Minute);

protected:
  // Привязка к TextBlock для времени (имя в BP должно быть TimeText)
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> TimeText;
};