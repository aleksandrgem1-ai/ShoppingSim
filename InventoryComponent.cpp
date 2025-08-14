// InventoryComponent.cpp (НОВЫЙ ФАЙЛ)

#include "InventoryComponent.h"
#include "ProductData.h"

UInventoryComponent::UInventoryComponent() {
  PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay() {
  Super::BeginPlay();

  // ВРЕМЕННО: Для теста автоматически "закупаем" 10 единиц тестового товара.
  // Позже это будет делать игрок через UI.
  // Убедитесь, что у вас есть Data Asset по этому пути!
  UProductData *TestProduct = Cast<UProductData>(
      StaticLoadObject(UProductData::StaticClass(), nullptr,
                       TEXT("/Game/Data/Products/DA_Milk.DA_Milk")));
  if (TestProduct) {
    Stock.Add(TestProduct, 10);
  }
}

int32 UInventoryComponent::SellRandomItem() {
  if (Stock.IsEmpty()) {
    return 0;
  }

  // Выбираем случайный товар из имеющихся
  TArray<TObjectPtr<UProductData>> Products;
  Stock.GetKeys(Products);
  UProductData *ProductToSell =
      Products[FMath::RandRange(0, Products.Num() - 1)];

  if (ProductToSell && Stock.Contains(ProductToSell)) {
    // Уменьшаем количество товара на 1
    Stock[ProductToSell]--;
    UE_LOG(LogTemp, Log, TEXT("Sold one %s. Stock left: %d"),
           *ProductToSell->ProductName.ToString(), Stock[ProductToSell]);

    // Если товар закончился, убираем его из списка
    if (Stock[ProductToSell] <= 0) {
      Stock.Remove(ProductToSell);
    }

    // Возвращаем выручку от продажи
    return ProductToSell->SalePrice;
  }

  return 0;
}

void UInventoryComponent::StockItem(UProductData *Product, int32 Quantity) {
  if (!Product || Quantity <= 0)
    return;

  // Добавляем товар на склад
  const int32 CurrentQuantity = Stock.FindOrAdd(Product);
  Stock.Add(Product, CurrentQuantity + Quantity);
}