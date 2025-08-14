// StoreZoneData.h (ОБНОВЛЕННЫЙ)

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StoreZoneData.generated.h"

class AStoreZoneActor; // Forward declaration

// Определяем типы зон
UENUM(BlueprintType)
enum class EZoneType : uint8 {
  // Одиночный предмет (полка, холодильник)
  Prop UMETA(DisplayName = "Prop"),

  // Зона-шаблон, которая содержит в себе другие предметы
  Template UMETA(DisplayName = "Template")
};

// Структура для описания дочерних объектов в шаблоне
USTRUCT(BlueprintType)
struct FSubItemInfo {
  GENERATED_BODY()

  // Какой Data Asset использовать для дочернего объекта
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  TObjectPtr<UStoreZoneData> SubItemData;

  // Позиция и поворот объекта относительно родителя (шаблона)
  UPROPERTY(EditAnywhere, BlueprintReadOnly)
  FTransform RelativeTransform;
};

UCLASS(BlueprintType)
class SHOPPINGSIM_API UStoreZoneData : public UPrimaryDataAsset {
  GENERATED_BODY()

public:
  // Тип этой зоны
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  EZoneType ZoneType = EZoneType::Prop;

  // Имя зоны, которое будет отображаться в UI
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  FText ZoneName;

  // Стоимость постройки
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  int32 Cost = 100;

  // Доход, который зона генерирует (если применимо)
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  float IncomePerSecond = 0.0f;

  // Визуальное представление (для шаблонов это может быть просто пол или
  // невидимый объект)
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  TSoftObjectPtr<UStaticMesh> ZoneMesh;

  // Иконка для UI
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data")
  TSoftObjectPtr<UTexture2D> ZoneIcon;

  // Массив дочерних объектов (используется, только если ZoneType = Template)
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone Data",
            meta = (EditCondition = "ZoneType == EZoneType::Template"))
  TArray<FSubItemInfo> TemplateItems;
};