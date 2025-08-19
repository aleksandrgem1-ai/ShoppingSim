// Subsystems/Private/FurniturePlaceManagerSubsystem.cpp

#include "Subsystems/FurniturePlaceManagerSubsystem.h"
#include "Data/FurnitureDataAsset.h"
#include "Actors/PlaceableActor.h"
#include "Actors/StoreZoneActor.h"

void UFurniturePlaceManagerSubsystem::Initialize(
    FSubsystemCollectionBase &Collection) {
  Super::Initialize(Collection);
  FurnitureToPlace = nullptr;
  bInPlacementMode = false;
  bMovingFurniture = false;
  MovingFurnitureActor = nullptr;
  PlacementHistory.Empty();
  HistoryIndex = -1;
}

void UFurniturePlaceManagerSubsystem::EnterPlacementMode(
    UFurnitureDataAsset *FurnitureData) {
  if (!FurnitureData)
    return;

  FurnitureToPlace = FurnitureData;
  bInPlacementMode = true;
  OnPlacementModeChanged.Broadcast(true);
}

void UFurniturePlaceManagerSubsystem::ExitPlacementMode() {
  FurnitureToPlace = nullptr;
  bInPlacementMode = false;
  bMovingFurniture = false;
  MovingFurnitureActor = nullptr;
  OnPlacementModeChanged.Broadcast(false);
}

bool UFurniturePlaceManagerSubsystem::PlaceFurnitureAt(
    AStoreZoneActor *ZoneActor, int32 SlotIndex) {
  if (!ZoneActor || !FurnitureToPlace)
    return false;

  APlaceableActor *NewFurniture =
      ZoneActor->GetWorld()->SpawnActor<APlaceableActor>();
  if (!NewFurniture)
    return false;

  NewFurniture->FurnitureDef = FurnitureToPlace;
  if (NewFurniture->PlaceInZone(ZoneActor, SlotIndex)) {
    FurnitureToPlace = nullptr;
    bInPlacementMode = false;
    OnFurniturePlaced.Broadcast(NewFurniture, ZoneActor);
    PushHistory(TEXT("Placed in slot"));
    return true;
  }

  NewFurniture->Destroy();
  return false;
}

bool UFurniturePlaceManagerSubsystem::PlaceFurnitureAtPosition(
    AStoreZoneActor *ZoneActor, const FVector &WorldPos) {
  if (!ZoneActor || !FurnitureToPlace)
    return false;

  APlaceableActor *NewFurniture =
      ZoneActor->GetWorld()->SpawnActor<APlaceableActor>();
  if (!NewFurniture)
    return false;

  NewFurniture->FurnitureDef = FurnitureToPlace;
  if (NewFurniture->PlaceAtPosition(ZoneActor, WorldPos)) {
    FurnitureToPlace = nullptr;
    bInPlacementMode = false;
    OnFurniturePlaced.Broadcast(NewFurniture, ZoneActor);
    PushHistory(TEXT("Placed at position"));
    return true;
  }

  NewFurniture->Destroy();
  return false;
}

void UFurniturePlaceManagerSubsystem::StartMoveFurniture(
    APlaceableActor *FurnitureActor) {
  if (!FurnitureActor)
    return;

  bMovingFurniture = true;
  MovingFurnitureActor = FurnitureActor;
  FurnitureToPlace = FurnitureActor->FurnitureDef;
  OnPlacementModeChanged.Broadcast(true);
}

bool UFurniturePlaceManagerSubsystem::RemoveFurnitureFromZone(
    APlaceableActor *FurnitureActor) {
  if (!FurnitureActor)
    return false;

  FurnitureActor->RemoveFromZone();
  PushHistory(TEXT("Removed from zone"));
  OnFurniturePlaced.Broadcast(FurnitureActor, nullptr);
  return true;
}

void UFurniturePlaceManagerSubsystem::UndoLastAction() {
  if (HistoryIndex >= 0 && PlacementHistory.Num() > 0) {
    // Реализовать откат операций по истории (доработка)
    HistoryIndex--;
  }
}

void UFurniturePlaceManagerSubsystem::RedoLastAction() {
  if (HistoryIndex + 1 < PlacementHistory.Num()) {
    HistoryIndex++;
    // Реализовать возврат операций по истории (доработка)
  }
}

UFurnitureDataAsset *
UFurniturePlaceManagerSubsystem::GetFurnitureToPlace() const {
  return FurnitureToPlace;
}

void UFurniturePlaceManagerSubsystem::PushHistory(const FString &Operation) {
  if (HistoryIndex + 1 < PlacementHistory.Num()) {
    PlacementHistory.SetNum(HistoryIndex + 1);
  }
  PlacementHistory.Add(Operation);
  HistoryIndex = PlacementHistory.Num() - 1;
}

void UFurniturePlaceManagerSubsystem::NotifyPlacementPreview(
    const FVector &Location) {
  OnPlacementPreviewUpdated.Broadcast(Location);
}
