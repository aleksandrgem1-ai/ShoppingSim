// ClockWidget.cpp
#include "UI/HUD/ClockWidget.h"
#include "Components/TextBlock.h"
#include "Subsystems/TimeManagerSubsystem.h"

void UClockWidget::NativeConstruct() {
  Super::NativeConstruct();

  if (UWorld *World = GetWorld()) {
    if (UTimeManagerSubsystem *TimeManager =
            World->GetSubsystem<UTimeManagerSubsystem>()) {
      TimeManager->OnTimeUpdated.AddDynamic(this, &UClockWidget::UpdateTime);
    }
  }
}

void UClockWidget::NativeDestruct() {
  if (UWorld *World = GetWorld()) {
    if (UTimeManagerSubsystem *TimeManager =
            World->GetSubsystem<UTimeManagerSubsystem>()) {
      TimeManager->OnTimeUpdated.RemoveDynamic(this, &UClockWidget::UpdateTime);
    }
  }
  Super::NativeDestruct();
}

void UClockWidget::UpdateTime(int32 Hour, int32 Minute) {
  if (TimeText) {
    const FString TimeString = FString::Printf(TEXT("%02d:%02d"), Hour, Minute);
    TimeText->SetText(FText::FromString(TimeString));
  }
}

void UClockWidget::SetTime(int32 Hour, int32 Minute) {
  UpdateTime(Hour, Minute);
}
