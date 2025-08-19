// InventoryComponent.cpp (final)
#include "Components/InventoryComponent.h"
#include "Data/ProductData.h"

UInventoryComponent::UInventoryComponent() {
  PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay() {
  Super::BeginPlay();

  // ������ ����������� (��� ������). �������/�������� �� ��������-������.
  if (UProductData *TestProduct = Cast<UProductData>(
          StaticLoadObject(UProductData::StaticClass(), nullptr,
                           TEXT("/Game/Data/Products/DA_Milk.DA_Milk")))) {
    Stock.Add(TestProduct, 10);
  }
}

int32 UInventoryComponent::SellRandomItem() {
  if (Stock.IsEmpty()) {
    return 0;
  }

  // ����� ������ ���� ��� �� �����, ��� � � TMap: TObjectPtr<UProductData>
  TArray<TObjectPtr<UProductData>> Products;
  Stock.GetKeys(Products);

  // ������ �� ������ ����������������
  if (Products.Num() == 0) {
    return 0;
  }

  UProductData *ProductToSell =
      Products[FMath::RandRange(0, Products.Num() - 1)].Get();
  if (!ProductToSell) {
    return 0;
  }

  if (int32 *CountPtr = Stock.Find(ProductToSell)) {
    int32 &Count = *CountPtr;
    if (Count <= 0) {
      // ������ �������� ������, ���� ����� ��������
      Stock.Remove(ProductToSell);
      return 0;
    }

    // ������ 1 �������
    Count--;

    UE_LOG(LogTemp, Log, TEXT("Sold one %s. Stock left: %d"),
           *ProductToSell->ProductName.ToString(), Count);

    if (Count <= 0) {
      Stock.Remove(ProductToSell);
    }

    // ���������� �������
    return ProductToSell->SalePrice;
  }

  return 0;
}

void UInventoryComponent::StockItem(UProductData *Product, int32 Quantity) {
  if (!Product || Quantity <= 0) {
    return;
  }

  // FindOrAdd ������� ������, ���� � �� ����
  int32 &CurrentQuantity = Stock.FindOrAdd(Product);
  CurrentQuantity += Quantity;
}

int32 UInventoryComponent::GetQuantity(const UProductData *Product) const {
  if (!Product) {
    return 0;
  }

  // Find �������� � ������ ����������; TObjectPtr ���� ��������� ����������
  const int32 *CountPtr = Stock.Find(const_cast<UProductData *>(Product));
  return CountPtr ? *CountPtr : 0;
}

void UInventoryComponent::GetAllProducts(
    TArray<TObjectPtr<UProductData>> &OutProducts) const {
  OutProducts.Reset();
  Stock.GetKeys(OutProducts);
}
