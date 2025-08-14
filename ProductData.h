// ProductData.h (����� ����)
// ����������� �� UPrimaryDataAsset

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProductData.generated.h"

UCLASS(BlueprintType)
class SHOPPINGSIM_API UProductData : public UPrimaryDataAsset {
  GENERATED_BODY()

public:
  // ��� ������ ��� UI
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Product")
  FText ProductName;

  // ����, �� ������� �� �������� ����� � ����������
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Product")
  int32 PurchasePrice = 10;

  // ����, �� ������� �� ������� ����� �����������
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Product")
  int32 SalePrice = 15;
};