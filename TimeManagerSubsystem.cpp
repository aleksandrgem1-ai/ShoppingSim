// TimeManagerSubsystem.cpp (Ќќ¬џ… ‘ј…Ћ)

#include "TimeManagerSubsystem.h"
#include "EconomySubsystem.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "StoreZoneActor.h"

void UTimeManagerSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
  Super::Initialize(Collection);
  GetWorld()->GetTimerManager().SetTimer(SecondTickTimerHandle, this,
                                         &UTimeManagerSubsystem::TickSecond,
                                         1.0f, true);
}

void UTimeManagerSubsystem::Deinitialize() {
  GetWorld()->GetTimerManager().ClearTimer(SecondTickTimerHandle);
  Super::Deinitialize();
}

void UTimeManagerSubsystem::TickSecond() {
  TimeElapsedInDay++;

  // --- 1. —»ћ”Ћя÷»я ѕ–ќƒј∆ ---
  TArray<AActor *> FoundZoneActors;
  UGameplayStatics::GetAllActorsOfClass(
      GetWorld(), AStoreZoneActor::StaticClass(), FoundZoneActors);

  if (FoundZoneActors.Num() > 0) {
    // „ем больше у вас полок, тем выше шанс продажи каждую секунду
    // (здесь можно добавить более сложную логику)
    if (FMath::RandRange(1, 100) <=
        (FoundZoneActors.Num() * 5)) // 5% шанс на полку
    {
      AStoreZoneActor *RandomZone = Cast<AStoreZoneActor>(
          FoundZoneActors[FMath::RandRange(0, FoundZoneActors.Num() - 1)]);
      if (RandomZone && RandomZone->GetInventoryComponent()) {
        const int32 Revenue =
            RandomZone->GetInventoryComponent()->SellRandomItem();
        if (Revenue > 0) {
          if (UEconomySubsystem *Economy =GetGameInstance()->GetSubsystem<UEconomySubsystem>()) {
            Economy->AddMoney(Revenue);
            CurrentDayIncome += Revenue;
            UE_LOG(LogTemp, Warning, TEXT("Sale! +%d money."), Revenue);
          }
        }
      }
    }
  }

  // --- 2. ѕроверка на конец дн€ ---
  if (TimeElapsedInDay >= DayDurationInSeconds) {
    const int32 PreviousDayIncome = CurrentDayIncome;
    const int32 PreviousDayGoal = CurrentDayGoal;

    CurrentDay++;
    TimeElapsedInDay = 0.0f;
    CurrentDayIncome = 0;
    CurrentDayGoal = FMath::RoundToInt(PreviousDayGoal * 1.5f);

    OnDayChanged.Broadcast(CurrentDay, PreviousDayIncome, PreviousDayGoal,
                           CurrentDayGoal);

    UE_LOG(LogTemp, Warning,
           TEXT("Day %d has ended. Income: %d, Goal: %d. New day: %d, New "
                "Goal: %d"),
           CurrentDay - 1, PreviousDayIncome, PreviousDayGoal, CurrentDay,
           CurrentDayGoal);
  }
}