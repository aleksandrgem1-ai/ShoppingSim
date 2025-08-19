// UI/HUD/FurnitureCatalogWidget.h

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "FurnitureCatalogWidget.generated.h"

class UListView; // Пример для списка
class UFurnitureDataAsset;
class UStoreInventorySubsystem;

/**
 * Виджет каталога мебели - список мебели, выбор для начала размещения
 */
UCLASS()
class SHOPPINGSIM_API UFurnitureCatalogWidget : public UUserWidget {
  GENERATED_BODY()

public:
  virtual void NativeConstruct() override;

  // Обновить список мебели из StoreInventorySubsystem
  UFUNCTION(BlueprintCallable, Category = "FurnitureCatalog")
  void RefreshFurnitureList();

protected:
  // Привязка к элементу UI из Blueprint
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UListView> FurnitureListView;

  // Кэш подсистемы
  UPROPERTY()
  TObjectPtr<UStoreInventorySubsystem> InventorySubsystem;

  // Внутренняя обработка выбора элемента из списка (можно реализовать через
  // Blueprint или C++)
  UFUNCTION()
  void OnFurnitureSelected(UObject *SelectedItem);
};
