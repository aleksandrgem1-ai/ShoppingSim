// UI/HUD/FurnitureDeliveryWidget.h

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "FurnitureDeliveryWidget.generated.h"

class UListView;
class UStoreInventorySubsystem;
class UProductData;

/**
 * Виджет отображения мебели на складе и в доставке
 */
UCLASS()
class SHOPPINGSIM_API UFurnitureDeliveryWidget : public UUserWidget {
  GENERATED_BODY()

public:
  virtual void NativeConstruct() override;

  UFUNCTION(BlueprintCallable, Category = "Delivery")
  void RefreshDeliveryList();

protected:
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UListView> DeliveryListView;

  UPROPERTY()
  TObjectPtr<UStoreInventorySubsystem> InventorySubsystem;
};
