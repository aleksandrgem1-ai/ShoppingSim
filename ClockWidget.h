// ShoppingSim/ClockWidget.h

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "ClockWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOPPINGSIM_API UClockWidget : public UUserWidget {
  GENERATED_BODY()

protected:
  virtual void NativeConstruct() override;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> TimeText;

  // днаюбкъел UFUNCTION()
  UFUNCTION()
  void UpdateTime(int32 Hour, int32 Minute);
};
