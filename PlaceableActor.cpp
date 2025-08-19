// PlaceableActor.cpp
#include "Actors/PlaceableActor.h"
#include "Actors/StoreZoneActor.h"

APlaceableActor::APlaceableActor() { PrimaryActorTick.bCanEverTick = false; }

bool APlaceableActor::PlaceInZone(AStoreZoneActor *Zone, int32 SlotIndex) {
  if (!Zone)
    return false;

  // Проверка индекса
  if (!Zone->IsValidSlot(SlotIndex))
    return false;

  // Проверка типа мебели и слота — логика в StoreZoneActor
  if (!Zone->CanPlaceFurnitureAtSlot(FurnitureDef->FurnitureType, SlotIndex))
    return false;

  // Проверка занятости слота
  if (Zone->IsSlotOccupied(SlotIndex))
    return false;

  // Закрепляем себя в зоне
  OwnerZone = Zone;
  ZoneSlotIndex = SlotIndex;
  InventoryState = EInventoryState::Placed;

  // Получаем локальную позицию слота и ставим актор
  FVector LocalPos = Zone->GetSlotPosition(SlotIndex);
  SetActorLocation(Zone->GetActorTransform().TransformPosition(LocalPos));

  // Помечаем слот занятым в зоне
  Zone->SetSlotOccupied(SlotIndex, true, this);

  return true;
}

bool APlaceableActor::PlaceAtPosition(AStoreZoneActor *Zone,
                                      const FVector &WorldPosition) {
  if (!Zone)
    return false;

  // Для кастомных зон, которые разрешают свободное размещение
  if (Zone->GetZoneType() != EZoneType::Custom)
    return false;

  OwnerZone = Zone;
  ZoneSlotIndex = -1;
  InventoryState = EInventoryState::Placed;

  SetActorLocation(WorldPosition);

  // В кастомных зонах слоты не используются
  return true;
}

void APlaceableActor::RemoveFromZone() {
  if (OwnerZone && ZoneSlotIndex != -1) {
    OwnerZone->SetSlotOccupied(ZoneSlotIndex, false, nullptr);
  }

  OwnerZone = nullptr;
  ZoneSlotIndex = -1;
  InventoryState = EInventoryState::InStock;

  // Можно спрятать объект, снять с мира и т.п.
  SetActorLocation(FVector::ZeroVector);
}

void APlaceableActor::BeginPlay() { Super::BeginPlay(); }
