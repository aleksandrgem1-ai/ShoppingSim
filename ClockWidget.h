// ClockWidget.h (����� ����)

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "ClockWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOPPINGSIM_API UClockWidget : public UUserWidget {
  GENERATED_BODY()

public:
  // ��������� ����� � ��������
  UFUNCTION(BlueprintCallable)
  void SetTime(int32 Hour, int32 Minute);

protected:
  // �������� � TextBlock ��� ������� (��� � BP ������ ���� TimeText)
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> TimeText;
};