// DayInfoWidget.cpp (ÍÎÂÛÉ ÔÀÉË)

#include "DayInfoWidget.h"
#include "Components/TextBlock.h"

void UDayInfoWidget::SetDayInfo(int32 Day, int32 Goal) {
  if (DayText) {
    const FString Formatted =
        FString::Printf(TEXT("Äåíü: %d | Öåëü: %dˆ"), Day, Goal);
    DayText->SetText(FText::FromString(Formatted));
  }
}