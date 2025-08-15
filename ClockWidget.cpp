// ClockWidget.cpp (ÍÎÂÛÉ ÔÀÉË)

#include "ClockWidget.h"
#include "Components/TextBlock.h"

void UClockWidget::SetTime(int32 Hour, int32 Minute) {
  if (TimeText) {
    const FString Formatted = FString::Printf(TEXT("%02d:%02d"), Hour, Minute);
    TimeText->SetText(FText::FromString(Formatted));
  }
}