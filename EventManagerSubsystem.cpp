// EventManagerSubsystem.cpp (НОВЫЙ ФАЙЛ)

#include "EventManagerSubsystem.h"
#include "EconomySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "StoreZoneActor.h"

void UEventManagerSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
  Super::Initialize(Collection);
  // Каждые 60 секунд будем пытаться запустить событие
  GetWorld()->GetTimerManager().SetTimer(
      EventTimerHandle, this, &UEventManagerSubsystem::TryTriggerRandomEvent,
      60.0f, true);
}

void UEventManagerSubsystem::Deinitialize() {
  GetWorld()->GetTimerManager().ClearTimer(EventTimerHandle);
  Super::Deinitialize();
}

void UEventManagerSubsystem::TryTriggerRandomEvent() {
  // 20% шанс, что событие произойдет
  if (FMath::RandRange(1, 5) == 1) {
    // Выбираем, какое именно событие запустить
    if (FMath::RandBool()) {
      TriggerBreakdownEvent();
    } else {
      TriggerFineEvent();
    }
  }
}

void UEventManagerSubsystem::TriggerBreakdownEvent() {
  TArray<AActor *> FoundZoneActors;
  UGameplayStatics::GetAllActorsOfClass(
      GetWorld(), AStoreZoneActor::StaticClass(), FoundZoneActors);

  // Убираем уже сломанные объекты из списка кандидатов
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
      UE_LOG(LogTemp, Warning, TEXT("%s"), *EventText.ToString());
    }
  }
}

void UEventManagerSubsystem::TriggerFineEvent() {
  if (UEconomySubsystem *Economy =
          GetWorld()->GetGameInstance()->GetSubsystem<UEconomySubsystem>()) {
    const int32 FineAmount = FMath::RandRange(50, 200);
    if (Economy->TrySpendMoney(FineAmount)) {
      FText EventText =
          FText::Format(NSLOCTEXT("GameEvents", "FineEvent",
                                  "Неожиданный штраф! Вы заплатили {0}€."),
                        FText::AsNumber(FineAmount));
      OnGameEvent.Broadcast(EventText);
      UE_LOG(LogTemp, Warning, TEXT("%s"), *EventText.ToString());
    }
  }
}