// StoreZoneData.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StoreZoneData.generated.h"

UCLASS(BlueprintType)
class SHOPPINGSIM_API UStoreZoneData : public UPrimaryDataAsset {
  GENERATED_BODY()

public:
  // ��� ����, ������� ����� ������������ � UI
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  FText ZoneName;

  // ��������� ��������� ���� ����
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  int32 Cost = 100;

  // �����, ������� ���� ���������� � �������
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  float IncomePerSecond = 1.0f;

  // ���������� ������������� ����
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  TSoftObjectPtr<UStaticMesh> ZoneMesh;

  // ������ ��� UI
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  TSoftObjectPtr<UTexture2D> ZoneIcon;
};