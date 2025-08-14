// InventoryComponent.h (����� ����)
// ����������� �� UActorComponent

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

  // �������� ������� ��������� ����� �� ���������. ���������� �������.
  int32 SellRandomItem();

  // (� �������) ������� ��� ���������� ������ �� �����
  void StockItem(UProductData *Product, int32 Quantity);

protected:
  virtual void BeginPlay() override;

  // "�����" ������ �������. ������, ����� ����� � � ����� ���������� �����.
  UPROPERTY(VisibleAnywhere, Category = "Inventory")
  TMap<TObjectPtr<UProductData>, int32> Stock;
};