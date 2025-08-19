#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Utils/FurnitureTypes.h"
#include "FurnitureDataAsset.generated.h"

UCLASS(BlueprintType)
class SHOPPINGSIM_API UFurnitureDataAsset : public UPrimaryDataAsset {
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Furniture")
  FText FurnitureName;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Furniture")
  EFurnitureType FurnitureType;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Furniture")
  int32 Cost = 100;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Furniture")
  int32 Capacity = 10;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Furniture")
  TSoftObjectPtr<UStaticMesh> FurnitureMesh;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Furniture")
  TSoftObjectPtr<UTexture2D> FurnitureIcon;
};
