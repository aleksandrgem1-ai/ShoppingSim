// ShoppingSim/TimeManagerSubsystem.cpp

#include "TimeManagerSubsystem.h"
#include "Curves/CurveFloat.h"
#include "EconomySubsystem.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SimSettings.h"
#include "StoreZoneActor.h"
#include "TimerManager.h"

void UTimeManagerSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
  Super::Initialize(Collection);

  // Load settings from project settings
  const USimSettings *Settings = GetDefault<USimSettings>();
  if (Settings) {
    DayDurationInSeconds = Settings->DayDurationInSeconds;
  }

  DemandCurve = Cast<UCurveFloat>(
      StaticLoadObject(UCurveFloat::StaticClass(), nullptr,
                       TEXT("/Game/Data/Curves/C_DemandCurve.C_DemandCurve")));

  // Start the 1-second timer
  if (UWorld *World = GetWorld()) {
    World->GetTimerManager().SetTimer(SecondTickTimerHandle, this,
                                      &UTimeManagerSubsystem::TickSecond, 1.0f,
                                      true);
  }
}

void UTimeManagerSubsystem::Deinitialize() {
  if (UWorld *World = GetWorld()) {
    World->GetTimerManager().ClearTimer(SecondTickTimerHandle);
  }
  Super::Deinitialize();
}

// This function is kept for compatibility with SimPlayerController's initial
// call
void UTimeManagerSubsystem::GetCurrentDayInfo(int &OutDay, int &OutHour) const {
  OutDay = CurrentDay;
  // Calculate hour based on day progress
  const float DayProgress =
      DayDurationInSeconds > 0 ? (TimeElapsedInDay / DayDurationInSeconds) : 0;
  OutHour = FMath::FloorToInt(DayProgress * 24.0f);
}

void UTimeManagerSubsystem::TickSecond() {
  TimeElapsedInDay++;

  const float DayProgress =
      DayDurationInSeconds > 0 ? (TimeElapsedInDay / DayDurationInSeconds) : 0;
  const int32 CurrentHour = FMath::FloorToInt(DayProgress * 24.0f);
  const int32 CurrentMinute =
      FMath::FloorToInt(FMath::Fmod(DayProgress * 24.0f * 60.0f, 60.0f));
  OnTimeUpdated.Broadcast(CurrentHour, CurrentMinute);

  TArray<AActor *> FoundZoneActors;
  UGameplayStatics::GetAllActorsOfClass(
      GetWorld(), AStoreZoneActor::StaticClass(), FoundZoneActors);
  FoundZoneActors.RemoveAll([](const AActor *Actor) {
    const AStoreZoneActor *Zone = Cast<AStoreZoneActor>(Actor);
    return Zone && Zone->IsBroken();
  });

  if (FoundZoneActors.Num() > 0 && DemandCurve) {
    const float DemandMultiplier = DemandCurve->GetFloatValue(DayProgress);
    const float BaseSaleChancePerZone = 5.0f; // This is chance per second
    const float CurrentSaleChance =
        FoundZoneActors.Num() * BaseSaleChancePerZone * DemandMultiplier;

    if (FMath::RandRange(0.f, 100.f) <= CurrentSaleChance) {
      AStoreZoneActor *RandomZone = Cast<AStoreZoneActor>(
          FoundZoneActors[FMath::RandRange(0, FoundZoneActors.Num() - 1)]);
      if (RandomZone && RandomZone->GetInventoryComponent()) {
        const int32 Revenue =
            RandomZone->GetInventoryComponent()->SellRandomItem();
        if (Revenue > 0) {
          if (UEconomySubsystem *Economy =
                  GetWorld()
                      ->GetGameInstance()
                      ->GetSubsystem<UEconomySubsystem>()) {
            Economy->AddMoney(Revenue);
            CurrentDayIncome += Revenue;
          }
        }
      }
    }
  }

  if (TimeElapsedInDay >= DayDurationInSeconds) {
    const int32 PreviousDayIncome = CurrentDayIncome;
    const int32 PreviousDayGoal = CurrentDayGoal;
    CurrentDay++;
    TimeElapsedInDay = 0.0f;
    CurrentDayIncome = 0;
    CurrentDayGoal = FMath::RoundToInt(PreviousDayGoal * 1.5f);
    OnDayChanged.Broadcast(CurrentDay, PreviousDayIncome, PreviousDayGoal,
                           CurrentDayGoal);
  }
}