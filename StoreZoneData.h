#pragma once

#include "CoreMinimal.h"
#include "Utils/FurnitureTypes.h"  // Подключаем ЕДИНСТВЕННОЕ определение EZoneType и FZoneSlot
#include "Engine/DataAsset.h"
#include "StoreZoneData.generated.h"

USTRUCT(BlueprintType)
struct FSubItemInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<class UStoreZoneData> SubItemData;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FTransform RelativeTransform;
};

UCLASS(BlueprintType)
class SHOPPINGSIM_API UStoreZoneData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    // Тип этой зоны (Custom или Template)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
    EZoneType ZoneType = EZoneType::Custom;

    // Имя зоны для UI
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
    FText ZoneName;

    // Стоимость постройки
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
    int32 Cost = 100;

    // Доход, который зона генерирует (если применимо)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
    float IncomePerSecond = 0.0f;

    // Визуальное представление
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
    TSoftObjectPtr<UStaticMesh> ZoneMesh;

    // Иконка для UI
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
    TSoftObjectPtr<UTexture2D> ZoneIcon;

    // Массив desc для шаблонных зон (Prefab) — если зона-компоновка из других зон
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data", meta = (EditCondition = "ZoneType == EZoneType::Template"))
    TArray<FSubItemInfo> TemplateItems;

    // Массив слотов для размещения мебели (только для шаблонов)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data", meta = (EditCondition = "ZoneType == EZoneType::Template"))
    TArray<FZoneSlot> ZoneSlots;
};
