#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h" // Правильный include для Entry
#include "FurnitureCatalogEntryWidget.generated.h"

class UImage;
class UTextBlock;
class UFurnitureDataAsset;

/**
 * Виджет элемента списка каталога мебели (Entry Widget)
 */
UCLASS()
class SHOPPINGSIM_API UFurnitureCatalogEntryWidget
    : public UUserWidget,
      public IUserObjectListEntry {
  GENERATED_BODY()

public:
  virtual void NativeOnListItemObjectSet(UObject *ListItemObject) override;

protected:
  UPROPERTY(meta = (BindWidget))
  UImage *FurnitureIcon;

  UPROPERTY(meta = (BindWidget))
  UTextBlock *FurnitureName;

  UPROPERTY()
  UFurnitureDataAsset *BoundFurniture;

  void UpdateEntry();
};
