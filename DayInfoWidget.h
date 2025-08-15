// DayInfoWidget.h (����� ����)

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "DayInfoWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOPPINGSIM_API UDayInfoWidget : public UUserWidget {
  GENERATED_BODY()

public:
  // ��������� ����� � ����������� � ���
  UFUNCTION(BlueprintCallable)
  void SetDayInfo(int32 Day, int32 Goal);

protected:
  // �������� � TextBlock ��� ��� (��� � BP ������ ���� DayText)
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> DayText;
};