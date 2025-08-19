// UI/HUD/Private/FurnitureDeliveryWidget.cpp

#include "UI/HUD/FurnitureDeliveryWidget.h"
#include "Components/ListView.h"
#include "Data/FurnitureDataAsset.h"
#include "Subsystems/StoreInventorySubsystem.h"

void UFurnitureDeliveryWidget::NativeConstruct() {
  Super::NativeConstruct();

  InventorySubsystem = GetWorld()
                           ? GetWorld()
                                 ->GetGameInstance()
                                 ->GetSubsystem<UStoreInventorySubsystem>()
                           : nullptr;

  RefreshDeliveryList();

  if (InventorySubsystem) {
    InventorySubsystem->OnInventoryChanged.AddDynamic(
        this, &UFurnitureDeliveryWidget::RefreshDeliveryList);
  }
}

void UFurnitureDeliveryWidget::RefreshDeliveryList() {
  if (!InventorySubsystem || !DeliveryListView)
    return;

  DeliveryListView->ClearListItems();

  TArray<UFurnitureDataAsset *>
      DeliveryFurniture; // Обратите внимание на тип массива!
  InventorySubsystem->GetFurnitureInDelivery(DeliveryFurniture);

  for (UFurnitureDataAsset *Furniture : DeliveryFurniture) {
    if (Furniture) {
      DeliveryListView->AddItem(Furniture);
    }
  }
}