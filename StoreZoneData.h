// StoreZoneData.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StoreZoneData.generated.h"

UCLASS(BlueprintType)
class SHOPPINGSIM_API UStoreZoneData : public UPrimaryDataAsset {
  GENERATED_BODY()

public:
  // Имя зоны, которое будет отображаться в UI
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  FText ZoneName;

  // Стоимость постройки этой зоны
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  int32 Cost = 100;

  // Доход, который зона генерирует в секунду
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  float IncomePerSecond = 1.0f;

  // Визуальное представление зоны
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  TSoftObjectPtr<UStaticMesh> ZoneMesh;

  // Иконка для UI
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  TSoftObjectPtr<UTexture2D> ZoneIcon;
};