// GameHUDWidget.h (�����������)

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameHUDWidget.generated.h"

class UMoneyHUDWidget; // <-- ���������� ��� �����
class UClockWidget;
class UDayInfoWidget;

UCLASS()
class SHOPPINGSIM_API UGameHUDWidget : public UUserWidget {
  GENERATED_BODY()

public:
  void SetMoney(int32 NewValue);
  void SetTime(int32 Hour, int32 Minute);
  void SetDayInfo(int32 Day, int32 Goal);

protected:
  // --- �������� � �������� � Blueprint ---

  // �������� � ������ ������� �����
  // (��� � BP ������ ���� MoneyWidgetInstance)
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UMoneyHUDWidget> MoneyWidgetInstance;

  // �������� � ������� �����
  // (��� � BP ������ ���� ClockWidgetInstance)
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UClockWidget> ClockWidgetInstance;

  // �������� � ������� ���������� � ���
  // (��� � BP ������ ���� DayInfoWidgetInstance)
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UDayInfoWidget> DayInfoWidgetInstance;
};