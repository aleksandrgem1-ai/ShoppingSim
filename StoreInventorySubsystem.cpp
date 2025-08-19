#include "Subsystems/StoreInventorySubsystem.h"
#include "Actors/PlaceableActor.h"
#include "Data/FurnitureDataAsset.h"

void UStoreInventorySubsystem::Initialize(
    FSubsystemCollectionBase &Collection) {
  Super::Initialize(Collection);
  FurnitureStock.Empty();
  FurnitureInDelivery.Empty();
}

void UStoreInventorySubsystem::AddFurnitureToStock(
    UFurnitureDataAsset *FurnitureData, int32 Quantity) {
  if (!FurnitureData || Quantity <= 0)
    return;
  int32 &StockCount = FurnitureStock.FindOrAdd(FurnitureData);
  StockCount += Quantity;
  NotifyInventoryChanged();
}

bool UStoreInventorySubsystem::StartFurnitureDelivery(
    UFurnitureDataAsset *FurnitureData, int32 Quantity) {
  if (!FurnitureData || Quantity <= 0)
    return false;
  int32 *StockCountPtr = FurnitureStock.Find(FurnitureData);
  if (!StockCountPtr || *StockCountPtr < Quantity)
    return false;

  *StockCountPtr -= Quantity;
  if (*StockCountPtr <= 0)
    FurnitureStock.Remove(FurnitureData);

  int32 &DeliveryCount = FurnitureInDelivery.FindOrAdd(FurnitureData);
  DeliveryCount += Quantity;

  NotifyInventoryChanged();
  return true;
}

bool UStoreInventorySubsystem::CompleteFurnitureDelivery(
    APlaceableActor *FurnitureActor, UFurnitureDataAsset *FurnitureData) {
  if (!FurnitureActor || !FurnitureData)
    return false;
  int32 *DeliveryCountPtr = FurnitureInDelivery.Find(FurnitureData);
  if (!DeliveryCountPtr || *DeliveryCountPtr <= 0)
    return false;

  *DeliveryCountPtr -= 1;
  if (*DeliveryCountPtr <= 0)
    FurnitureInDelivery.Remove(FurnitureData);

  OnFurniturePlaced.Broadcast(FurnitureActor);
  NotifyInventoryChanged();
  return true;
}

void UStoreInventorySubsystem::CancelFurnitureDelivery(
    UFurnitureDataAsset *FurnitureData, int32 Quantity) {
  if (!FurnitureData || Quantity <= 0)
    return;
  int32 *DeliveryCountPtr = FurnitureInDelivery.Find(FurnitureData);
  if (!DeliveryCountPtr || *DeliveryCountPtr < Quantity)
    return;

  *DeliveryCountPtr -= Quantity;
  if (*DeliveryCountPtr <= 0)
    FurnitureInDelivery.Remove(FurnitureData);

  int32 &StockCount = FurnitureStock.FindOrAdd(FurnitureData);
  StockCount += Quantity;
  NotifyInventoryChanged();
}

int32 UStoreInventorySubsystem::GetStockQuantity(
    UFurnitureDataAsset *FurnitureData) const {
  if (!FurnitureData)
    return 0;
  const int32 *StockCountPtr = FurnitureStock.Find(FurnitureData);
  return StockCountPtr ? *StockCountPtr : 0;
}

int32 UStoreInventorySubsystem::GetDeliveryQuantity(
    UFurnitureDataAsset *FurnitureData) const {
  if (!FurnitureData)
    return 0;
  const int32 *DeliveryCountPtr = FurnitureInDelivery.Find(FurnitureData);
  return DeliveryCountPtr ? *DeliveryCountPtr : 0;
}

void UStoreInventorySubsystem::NotifyInventoryChanged() {
  OnInventoryChanged.Broadcast();
}

void UStoreInventorySubsystem::GetAllFurnitureAssets(
    TArray<UFurnitureDataAsset *> &OutList) const {
  OutList.Empty();
  for (const TPair<TObjectPtr<UFurnitureDataAsset>, int32> &Pair :
       FurnitureStock) {
    if (Pair.Key)
      OutList.Add(Pair.Key.Get());
  }
}

void UStoreInventorySubsystem::GetFurnitureInDelivery(
    TArray<UFurnitureDataAsset *> &OutList) const {
  OutList.Empty();
  for (const TPair<TObjectPtr<UFurnitureDataAsset>, int32> &Pair :
       FurnitureInDelivery) {
    if (Pair.Key)
      OutList.Add(Pair.Key.Get());
  }
}
