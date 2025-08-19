#include "Subsystems/EventManagerSubsystem.h"
#include "Actors/StoreZoneActor.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/SimSettings.h"
#include "Subsystems/EconomySubsystem.h"

void UEventManagerSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
  Super::Initialize(Collection);

  const USimSettings *Settings = GetDefault<USimSettings>();
  float EventInterval = Settings ? Settings->RandomEventInterval : 60.0f;

  UWorld *World = GetWorld();
  if (!World) {
    UE_LOG(LogTemp, Error, TEXT("[EMS] Initialize: World is null!"));
    return;
  }

  UE_LOG(LogTemp, Warning, TEXT("[EMS] Initialize in world: %s"),
         *World->GetName());

  World->GetTimerManager().SetTimer(
      EventTimerHandle, this, &UEventManagerSubsystem::TryTriggerRandomEvent,
      EventInterval, true);

  UE_LOG(LogTemp, Warning, TEXT("[EMS] Timer started (interval=%fs)"),
         EventInterval);
}

void UEventManagerSubsystem::Deinitialize() {
  UWorld *World = GetWorld();

  if (World) {
    World->GetTimerManager().ClearTimer(EventTimerHandle);
    UE_LOG(LogTemp, Warning, TEXT("[EMS] Deinitialize in world: %s"),
           *World->GetName());
  } else {
    UE_LOG(LogTemp, Error, TEXT("[EMS] Deinitialize: World is null!"));
  }

  Super::Deinitialize();
}

void UEventManagerSubsystem::EM_TestEvent() {
  UE_LOG(LogTemp, Warning, TEXT("[EMS] EM_TestEvent called from console"));
  TryTriggerRandomEvent();
}

void UEventManagerSubsystem::TryTriggerRandomEvent() {
  const USimSettings *Settings = GetDefault<USimSettings>();
  float EventChance = Settings ? Settings->RandomEventChance : 0.2f;

  UE_LOG(LogTemp, Log, TEXT("[EMS] TryTriggerRandomEvent (chance: %.2f)"),
         EventChance);

  if (FMath::FRand() <= EventChance) {
    if (FMath::RandBool()) {
      TriggerBreakdownEvent();
    } else {
      TriggerFineEvent();
    }
  } else {
    UE_LOG(LogTemp, Verbose, TEXT("[EMS] No event this tick"));
  }
}

void UEventManagerSubsystem::TriggerBreakdownEvent() {
  UWorld *World = GetWorld();
  if (!World) {
    UE_LOG(LogTemp, Error, TEXT("[EMS] TriggerBreakdownEvent: World is null!"));
    return;
  }

  TArray<AActor *> FoundZoneActors;
  UGameplayStatics::GetAllActorsOfClass(World, AStoreZoneActor::StaticClass(),
                                        FoundZoneActors);

  FoundZoneActors.RemoveAll([](const AActor *Actor) {
    const AStoreZoneActor *Zone = Cast<AStoreZoneActor>(Actor);
    return Zone && Zone->IsBroken();
  });

  if (FoundZoneActors.Num() > 0) {
    AStoreZoneActor *ZoneToBreak = Cast<AStoreZoneActor>(
        FoundZoneActors[FMath::RandRange(0, FoundZoneActors.Num() - 1)]);

    if (ZoneToBreak) {
      ZoneToBreak->BreakDown();

      FText EventText = FText::Format(
          NSLOCTEXT("GameEvents", "BreakdownEvent",
                    "Оборудование '{0}' сломалось! Требуется ремонт."),
          ZoneToBreak->GetZoneName());

      OnGameEvent.Broadcast(EventText);
      UE_LOG(LogTemp, Warning, TEXT("[EMS] %s"), *EventText.ToString());
    } else {
      UE_LOG(LogTemp, Error,
             TEXT("[EMS] ZoneToBreak is null after random select!"));
    }
  } else {
    UE_LOG(LogTemp, Verbose,
           TEXT("[EMS] No valid zones available for breakdown event."));
  }
}

void UEventManagerSubsystem::TriggerFineEvent() {
  UWorld *World = GetWorld();
  if (!World) {
    UE_LOG(LogTemp, Error, TEXT("[EMS] TriggerFineEvent: World is null!"));
    return;
  }

  UGameInstance *GI = World->GetGameInstance();
  if (!GI) {
    UE_LOG(LogTemp, Error,
           TEXT("[EMS] TriggerFineEvent: GameInstance is null!"));
    return;
  }

  UEconomySubsystem *Economy = GI->GetSubsystem<UEconomySubsystem>();
  if (!Economy) {
    UE_LOG(LogTemp, Error,
           TEXT("[EMS] TriggerFineEvent: EconomySubsystem is null!"));
    return;
  }

  const int32 FineAmount = FMath::RandRange(50, 200);

  if (Economy->TrySpendMoney(FineAmount)) {
    FText EventText =
        FText::Format(NSLOCTEXT("GameEvents", "FineEvent",
                                "Неожиданный штраф! Вы заплатили {0}И."),
                      FText::AsNumber(FineAmount));
    OnGameEvent.Broadcast(EventText);
    UE_LOG(LogTemp, Warning, TEXT("[EMS] %s"), *EventText.ToString());
  } else {
    UE_LOG(LogTemp, Warning, TEXT("[EMS] Fine %dИ skipped: not enough funds"),
           FineAmount);
  }
}
