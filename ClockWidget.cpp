#include "ClockWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "TimeManagerSubsystem.h"

void UClockWidget::NativeConstruct() {
  Super::NativeConstruct();

  if (UWorld *World = GetWorld()) {
    // Было:
    // if (UTimeManagerSubsystem* TimeManager =
    //        World->GetGameInstance()->GetSubsystem<UTimeManagerSubsystem>())
    if (UTimeManagerSubsystem *TimeManager =
            World->GetSubsystem<UTimeManagerSubsystem>()) {
      TimeManager->OnTimeUpdated.AddDynamic(this, &UClockWidget::UpdateTime);
    }
  }
}

void UClockWidget::UpdateTime(int32 Hour, int32 Minute) {
  if (TimeText) {
    const FString TimeString = FString::Printf(TEXT("%02d:%02d"), Hour, Minute);
    TimeText->SetText(FText::FromString(TimeString));
  }
}

void UClockWidget::SetTime(int32 Hour, int32 Minute) {
  // Можно просто переиспользовать существующую логику
  UpdateTime(Hour, Minute);
}
