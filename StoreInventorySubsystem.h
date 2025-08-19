#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Utils/FurnitureTypes.h"
#include "StoreInventorySubsystem.generated.h"

class UFurnitureDataAsset;
class APlaceableActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFurnitureStateChanged,
                                            APlaceableActor *, FurnitureActor);

/**
 * Подсистема управления мебелью: склад, магазин, доставка
 */
UCLASS()
class SHOPPINGSIM_API UStoreInventorySubsystem : public UGameInstanceSubsystem {
  GENERATED_BODY()

public:
  virtual void Initialize(FSubsystemCollectionBase &Collection) override;

  UFUNCTION(BlueprintCallable, Category = "Inventory")
  void AddFurnitureToStock(UFurnitureDataAsset *FurnitureData, int32 Quantity);

  UFUNCTION(BlueprintCallable, Category = "Inventory")
  bool StartFurnitureDelivery(UFurnitureDataAsset *FurnitureData,
                              int32 Quantity);

  UFUNCTION(BlueprintCallable, Category = "Inventory")
  bool CompleteFurnitureDelivery(APlaceableActor *FurnitureActor,
                                 UFurnitureDataAsset *FurnitureData);

  UFUNCTION(BlueprintCallable, Category = "Inventory")
  void CancelFurnitureDelivery(UFurnitureDataAsset *FurnitureData,
                               int32 Quantity);

  UFUNCTION(BlueprintCallable, Category = "Inventory")
  int32 GetStockQuantity(UFurnitureDataAsset *FurnitureData) const;

  UFUNCTION(BlueprintCallable, Category = "Inventory")
  int32 GetDeliveryQuantity(UFurnitureDataAsset *FurnitureData) const;

  // Делегаты для UI/логики
  UPROPERTY(BlueprintAssignable, Category = "Inventory")
  FOnInventoryChanged OnInventoryChanged;

  UPROPERTY(BlueprintAssignable, Category = "Inventory")
  FOnFurnitureStateChanged OnFurniturePlaced;

  // Вернуть все ассеты мебели на складе
  UFUNCTION(BlueprintCallable, Category = "Inventory")
  void GetAllFurnitureAssets(TArray<UFurnitureDataAsset *> &OutList) const;

  // Вернуть все ассеты мебели в доставке
  UFUNCTION(BlueprintCallable, Category = "Inventory")
  void GetFurnitureInDelivery(TArray<UFurnitureDataAsset *> &OutList) const;

private:
  UPROPERTY()
  TMap<TObjectPtr<UFurnitureDataAsset>, int32> FurnitureStock;

  UPROPERTY()
  TMap<TObjectPtr<UFurnitureDataAsset>, int32> FurnitureInDelivery;

  void NotifyInventoryChanged();
};
