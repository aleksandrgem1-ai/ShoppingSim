// UI/HUD/Private/FurnitureCatalogWidget.cpp

#include "UI/HUD/FurnitureCatalogWidget.h"
#include "Components/ListView.h"
#include "Data/FurnitureDataAsset.h"
#include "Subsystems/StoreInventorySubsystem.h"

void UFurnitureCatalogWidget::NativeConstruct() {
  Super::NativeConstruct();

  InventorySubsystem = GetWorld()
                           ? GetWorld()
                                 ->GetGameInstance()
                                 ->GetSubsystem<UStoreInventorySubsystem>()
                           : nullptr;

  if (FurnitureListView) {
    FurnitureListView->OnItemClicked().AddUObject(
        this, &UFurnitureCatalogWidget::OnFurnitureSelected);
  }

  RefreshFurnitureList();
}

void UFurnitureCatalogWidget::RefreshFurnitureList() {
  if (!InventorySubsystem || !FurnitureListView)
    return;

  FurnitureListView->ClearListItems();

  TArray<UFurnitureDataAsset *>
      FurnitureAssets; // Обратите внимание на тип массива!
  InventorySubsystem->GetAllFurnitureAssets(FurnitureAssets);

  for (UFurnitureDataAsset *Furniture : FurnitureAssets) {
    if (Furniture) {
      FurnitureListView->AddItem(Furniture);
    }
  }
}

void UFurnitureCatalogWidget::OnFurnitureSelected(UObject *SelectedItem) {
  if (UFurnitureDataAsset *Furniture =
          Cast<UFurnitureDataAsset>(SelectedItem)) {
    // Вызываем логику размещения мебели, например
    // FurniturePlaceManagerSubsystem->EnterPlacementMode(Furniture);
    // Можно реализовать через BlueprintImplementableEvent для гибкости
  }
}
