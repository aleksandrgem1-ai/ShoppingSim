// InventoryComponent.h (НОВЫЙ ФАЙЛ)
// Наследовать от UActorComponent

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

  // Пытается продать случайный товар из инвентаря. Возвращает выручку.
  int32 SellRandomItem();

  // (В будущем) Функция для добавления товара на полку
  void StockItem(UProductData *Product, int32 Quantity);

protected:
  virtual void BeginPlay() override;

  // "Склад" нашего объекта. Хранит, какой товар и в каком количестве лежит.
  UPROPERTY(VisibleAnywhere, Category = "Inventory")
  TMap<TObjectPtr<UProductData>, int32> Stock;
};