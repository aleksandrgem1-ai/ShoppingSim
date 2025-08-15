// ShoppingSim/DayInfoWidget.cpp

#include "DayInfoWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "TimeManagerSubsystem.h"

void UDayInfoWidget::NativeConstruct() {
  Super::NativeConstruct();

  if (UWorld *World = GetWorld()) {
    if (UTimeManagerSubsystem *TimeManager =
            World->GetGameInstance()->GetSubsystem<UTimeManagerSubsystem>()) {
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
  if (IncomeText) {
    IncomeText->SetText(FText::FromString(
        FString::Printf(TEXT("Income: %d"), 0))); // Start of day, income is 0
  }
  if (GoalText) {
    GoalText->SetText(
        FText::FromString(FString::Printf(TEXT("Goal: %d"), NewDayGoal)));
  }
}