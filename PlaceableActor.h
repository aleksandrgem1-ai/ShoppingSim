// PlaceableActor.h
#pragma once

#include "CoreMinimal.h"
#include "Data/FurnitureDataAsset.h"
#include "Utils/FurnitureTypes.h"
#include "GameFramework/Actor.h"
#include "PlaceableActor.generated.h"

UCLASS()
class SHOPPINGSIM_API APlaceableActor : public AActor {
  GENERATED_BODY()

public:
  APlaceableActor();

  /** Настройки мебели из DataAsset */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Furniture")
  TObjectPtr<UFurnitureDataAsset> FurnitureDef;

  /** Индекс слота, если размещён в слоте (\-1 если свободная позиция) */
  UPROPERTY(BlueprintReadOnly, Category = "Furniture")
  int32 ZoneSlotIndex = -1;

  /** Зона, к которой принадлежит мебель */
  UPROPERTY(BlueprintReadOnly, Category = "Furniture")
  TObjectPtr<class AStoreZoneActor> OwnerZone;

  /** Текущее состояние в инвентаре */
  UPROPERTY(BlueprintReadWrite, Category = "Furniture")
  EInventoryState InventoryState = EInventoryState::InStock;

  /** Попытаться разместить в слоте зоны */
  virtual bool PlaceInZone(class AStoreZoneActor *Zone, int32 SlotIndex);

  /** Разместить в произвольной позиции (для кастомных зон) */
  virtual bool PlaceAtPosition(class AStoreZoneActor *Zone,
                               const FVector &WorldPosition);

  /** Убрать из зоны, отправить обратно на склад */
  virtual void RemoveFromZone();

protected:
  virtual void BeginPlay() override;
};
