// MallSimSaveGame.h 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MallSimSaveGame.generated.h"

// Структура для хранения данных о размещенной зоне
USTRUCT(BlueprintType)
struct FZoneSaveData {
  GENERATED_BODY()

  // "Мягкий" путь к ассету DataAsset, чтобы сохранение было надежным
  UPROPERTY()
  FSoftObjectPath ZoneDataPath;

  // Позиция, поворот и масштаб зоны в мире
  UPROPERTY()
  FTransform ZoneTransform;
};

UCLASS()
class SHOPPINGSIM_API UMallSimSaveGame : public USaveGame {
  GENERATED_BODY()

public:
  UPROPERTY()
  int32 PlayerMoney;

  UPROPERTY()
  TArray<FZoneSaveData> PlacedZones;

  UPROPERTY()
  FString SaveSlotName = TEXT("DefaultSaveSlot");

  UPROPERTY()
  uint32 UserIndex = 0;
};