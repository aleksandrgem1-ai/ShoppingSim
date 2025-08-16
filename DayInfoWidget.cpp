#include "DayInfoWidget.h"
#include "Components/TextBlock.h"
#include "TimeManagerSubsystem.h"

void UDayInfoWidget::NativeConstruct() {
  Super::NativeConstruct();

  if (UWorld *World = GetWorld()) {
    if (UTimeManagerSubsystem *TimeManager =
            World->GetSubsystem<UTimeManagerSubsystem>()) {
      TimeManager->OnDayChanged.AddDynamic(this,
                                           &UDayInfoWidget::UpdateDayInfo);
    }
  }
}

void UDayInfoWidget::UpdateDayInfo(int32 NewDay, int32 PreviousDayIncome,
                                   int32 PreviousDayGoal, int32 NewDayGoal) {
  if (DayText) {
    DayText->SetText(
        FText::FromString(FString::Printf(TEXT("Day: %d"), NewDay)));
  }
  if (GoalText) {
    GoalText->SetText(
        FText::FromString(FString::Printf(TEXT("Goal: %d"), NewDayGoal)));
  }
  // Доход теперь обновляется отдельным событием SetIncome, хардкод убран
}

void UDayInfoWidget::SetDayInfo(int32 Day, int32 Goal) {
  if (DayText) {
    DayText->SetText(FText::FromString(FString::Printf(TEXT("Day: %d"), Day)));
  }
  if (GoalText) {
    GoalText->SetText(
        FText::FromString(FString::Printf(TEXT("Goal: %d"), Goal)));
  }
}

void UDayInfoWidget::SetIncome(int32 CurrentIncome) {
  if (IncomeText) {
    IncomeText->SetText(
        FText::FromString(FString::Printf(TEXT("Income: %d"), CurrentIncome)));
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[HUD] IncomeText is nullptr"));
  }
}
