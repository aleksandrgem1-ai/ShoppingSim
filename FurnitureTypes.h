#pragma once

#include "CoreMinimal.h"
#include "FurnitureTypes.generated.h"

UENUM(BlueprintType)
enum class EFurnitureType : uint8
{
    Shelf     UMETA(DisplayName = "Shelf"),
    Freezer   UMETA(DisplayName = "Freezer"),
    Counter   UMETA(DisplayName = "Counter"),
    Display   UMETA(DisplayName = "Display"),
    Other     UMETA(DisplayName = "Other")
};

UENUM(BlueprintType)
enum class EZoneType : uint8
{
    Custom    UMETA(DisplayName = "Custom"),
    Template  UMETA(DisplayName = "Template")
};

UENUM(BlueprintType)
enum class EInventoryState : uint8
{
    InStock      UMETA(DisplayName = "In Stock"),
    InDelivery   UMETA(DisplayName = "In Delivery"),
    Placed       UMETA(DisplayName = "Placed"),
    Removed      UMETA(DisplayName = "Removed")
};

USTRUCT(BlueprintType)
struct FZoneSlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Slot")
    FVector LocalPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Slot")
    EFurnitureType AllowedFurnitureType = EFurnitureType::Shelf;

    UPROPERTY(BlueprintReadOnly, Category = "Zone Slot")
    bool bOccupied = false;

    UPROPERTY(BlueprintReadOnly, Category = "Zone Slot")
    int32 SlotIndex = -1;
};
