#include "UI/HUD/FurnitureCatalogEntryWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/FurnitureDataAsset.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

void UFurnitureCatalogEntryWidget::NativeOnListItemObjectSet(
    UObject *ListItemObject) {
  BoundFurniture = Cast<UFurnitureDataAsset>(ListItemObject);
  UpdateEntry();
}

void UFurnitureCatalogEntryWidget::UpdateEntry() {
  if (!BoundFurniture)
    return;

  // Обновляем название
  if (FurnitureName) {
    FurnitureName->SetText(BoundFurniture->FurnitureName);
  }

  // Асинхронная загрузка иконки
  if (FurnitureIcon && !BoundFurniture->FurnitureIcon.IsNull()) {
    FStreamableManager &Streamable = UAssetManager::GetStreamableManager();
    const FSoftObjectPath &IconPath =
        BoundFurniture->FurnitureIcon.ToSoftObjectPath();

    Streamable.RequestAsyncLoad(IconPath, [this, IconPath]() {
      UTexture2D *LoadedIcon = Cast<UTexture2D>(IconPath.ResolveObject());
      if (FurnitureIcon && LoadedIcon) {
        FurnitureIcon->SetBrushFromTexture(LoadedIcon);
      }
    });
  }
}
