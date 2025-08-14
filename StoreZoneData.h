// StoreZoneData.h (�����������)

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StoreZoneData.generated.h"

class AStoreZoneActor; // Forward declaration

// ���������� ���� ���
UENUM(BlueprintType)
enum class EZoneType : uint8 {
  // ��������� ������� (�����, �����������)
  Prop UMETA(DisplayName = "Prop"),

  // ����-������, ������� �������� � ���� ������ ��������
  Template UMETA(DisplayName = "Template")
};

// ��������� ��� �������� �������� �������� � �������
USTRUCT(BlueprintType)
struct FSubItemInfo {
  GENERATED_BODY()

  // ����� Data Asset ������������ ��� ��������� �������
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TObjectPtr<UStoreZoneData> SubItemData;

  // ������� � ������� ������� ������������ �������� (�������)
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FTransform RelativeTransform;
};

UCLASS(BlueprintType)
class SHOPPINGSIM_API UStoreZoneData : public UPrimaryDataAsset {
  GENERATED_BODY()

public:
  // ��� ���� ����
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  EZoneType ZoneType = EZoneType::Prop;

  // ��� ����, ������� ����� ������������ � UI
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  FText ZoneName;

  // ��������� ���������
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  int32 Cost = 100;

  // �����, ������� ���� ���������� (���� ���������)
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  float IncomePerSecond = 0.0f;

  // ���������� ������������� (��� �������� ��� ����� ���� ������ ��� ���
  // ��������� ������)
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  TSoftObjectPtr<UStaticMesh> ZoneMesh;

  // ������ ��� UI
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  TSoftObjectPtr<UTexture2D> ZoneIcon;

  // ������ �������� �������� (������������, ������ ���� ZoneType = Template)
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data",
            meta = (EditCondition = "ZoneType == EZoneType::Template"))
  TArray<FSubItemInfo> TemplateItems;
};