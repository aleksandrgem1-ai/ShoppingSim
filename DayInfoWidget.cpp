// DayInfoWidget.cpp (����� ����)

#include "DayInfoWidget.h"
#include "Components/TextBlock.h"

void UDayInfoWidget::SetDayInfo(int32 Day, int32 Goal) {
  if (DayText) {
    const FString Formatted =
        FString::Printf(TEXT("����: %d | ����: %d�"), Day, Goal);
    DayText->SetText(FText::FromString(Formatted));
  }
}