// InventoryComponent.h (final)
#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "InventoryComponent.generated.h"

class UProductData;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOPPINGSIM_API UInventoryComponent : public UActorComponent {
  GENERATED_BODY()

public:
  UInventoryComponent();

  // Пытается продать случайный товар из инвентаря. Возвращает выручку (0, если
  // ничего не продано).
  int32 SellRandomItem();

  // Добавить товар на склад (увеличить количество).
  void StockItem(UProductData *Product, int32 Quantity);

  // Текущее количество товара (0, если нет).
  int32 GetQuantity(const UProductData *Product) const;

  // Полный список наименований (ключи инвентаря).
  void GetAllProducts(TArray<TObjectPtr<UProductData>> &OutProducts) const;

protected:
  virtual void BeginPlay() override;

  // Склад: Товар -> Количество
  UPROPERTY(VisibleAnywhere, Category = "Inventory")
  TMap<TObjectPtr<UProductData>, int32> Stock;
};
