// InventoryComponent.cpp (����� ����)

#include "InventoryComponent.h"
#include "ProductData.h"

UInventoryComponent::UInventoryComponent() {
  PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay() {
  Super::BeginPlay();

  // ��������: ��� ����� ������������� "��������" 10 ������ ��������� ������.
  // ����� ��� ����� ������ ����� ����� UI.
  // ���������, ��� � ��� ���� Data Asset �� ����� ����!
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

  // �������� ��������� ����� �� ���������
  TArray<TObjectPtr<UProductData>> Products;
  Stock.GetKeys(Products);
  UProductData *ProductToSell =
      Products[FMath::RandRange(0, Products.Num() - 1)];

  if (ProductToSell && Stock.Contains(ProductToSell)) {
    // ��������� ���������� ������ �� 1
    Stock[ProductToSell]--;
    UE_LOG(LogTemp, Log, TEXT("Sold one %s. Stock left: %d"),
           *ProductToSell->ProductName.ToString(), Stock[ProductToSell]);

    // ���� ����� ����������, ������� ��� �� ������
    if (Stock[ProductToSell] <= 0) {
      Stock.Remove(ProductToSell);
    }

    // ���������� ������� �� �������
    return ProductToSell->SalePrice;
  }

  return 0;
}

void UInventoryComponent::StockItem(UProductData *Product, int32 Quantity) {
  if (!Product || Quantity <= 0)
    return;

  // ��������� ����� �� �����
  const int32 CurrentQuantity = Stock.FindOrAdd(Product);
  Stock.Add(Product, CurrentQuantity + Quantity);
}