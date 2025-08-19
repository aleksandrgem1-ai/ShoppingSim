// TimeManagerSubsystem.cpp (fixed minimal)
#include "Subsystems/TimeManagerSubsystem.h"

#include "Curves/CurveFloat.h"
#include "Subsystems/EconomySubsystem.h"
#include "Components/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/SimSettings.h"
#include "Actors/StoreZoneActor.h"

void UTimeManagerSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
  Super::Initialize(Collection);

  if (const USimSettings *Settings = GetDefault<USimSettings>()) {
    DayDurationInSeconds = FMath::Max(60.f, Settings->DayDurationInSeconds);
  }

  DemandCurve = Cast<UCurveFloat>(
      StaticLoadObject(UCurveFloat::StaticClass(), nullptr,
                       TEXT("/Game/Data/Curves/C_DemandCurve.C_DemandCurve")));

  const float TickInterval =
      1.0f; // ����� �������� �� Settings->TimeTickInterval ��� �������
  GetWorld()->GetTimerManager().SetTimer(SecondTickTimerHandle, this,
                                         &UTimeManagerSubsystem::TickSecond,
                                         TickInterval, true);
}

void UTimeManagerSubsystem::Deinitialize() {
  GetWorld()->GetTimerManager().ClearTimer(SecondTickTimerHandle);
  Super::Deinitialize();
}

void UTimeManagerSubsystem::TickSecond() {
  TimeElapsedInDay++;
  const float DayProgress = TimeElapsedInDay / DayDurationInSeconds;
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
    const float BaseSaleChancePerZone = 5.0f;
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

void UTimeManagerSubsystem::GetCurrentDayInfo(int32 &OutDay,
                                              int32 &OutGoal) const {
  OutDay = CurrentDay;
  OutGoal = CurrentDayGoal;
}
