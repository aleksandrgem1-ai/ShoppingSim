// ProductData.h (НОВЫЙ ФАЙЛ)
// Наследовать от UPrimaryDataAsset 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProductData.generated.h"

UCLASS(BlueprintType)
class SHOPPINGSIM_API UProductData : public UPrimaryDataAsset {
  GENERATED_BODY()

public:
  // Имя товара для UI
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Product")
  FText ProductName;

  // Цена, по которой мы закупаем товар у поставщика
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Product")
  int32 PurchasePrice = 10;

  // Цена, по которой мы продаем товар покупателям
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Product")
  int32 SalePrice = 15;
};