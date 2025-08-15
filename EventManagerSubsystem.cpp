// EventManagerSubsystem.cpp (����� ����)

#include "EventManagerSubsystem.h"
#include "EconomySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "StoreZoneActor.h"

void UEventManagerSubsystem::Initialize(FSubsystemCollectionBase &Collection) {
  Super::Initialize(Collection);
  // ������ 60 ������ ����� �������� ��������� �������
  GetWorld()->GetTimerManager().SetTimer(
      EventTimerHandle, this, &UEventManagerSubsystem::TryTriggerRandomEvent,
      60.0f, true);
}

void UEventManagerSubsystem::Deinitialize() {
  GetWorld()->GetTimerManager().ClearTimer(EventTimerHandle);
  Super::Deinitialize();
}

void UEventManagerSubsystem::TryTriggerRandomEvent() {
  // 20% ����, ��� ������� ����������
  if (FMath::RandRange(1, 5) == 1) {
    // ��������, ����� ������ ������� ���������
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

  // ������� ��� ��������� ������� �� ������ ����������
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
                    "������������ '{0}' ���������! ��������� ������."),
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
                                  "����������� �����! �� ��������� {0}�."),
                        FText::AsNumber(FineAmount));
      OnGameEvent.Broadcast(EventText);
      UE_LOG(LogTemp, Warning, TEXT("%s"), *EventText.ToString());
    }
  }
}