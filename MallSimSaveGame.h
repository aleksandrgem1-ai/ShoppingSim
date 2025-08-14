// MallSimSaveGame.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MallSimSaveGame.generated.h"

// ��������� ��� �������� ������ � ����������� ����
USTRUCT(BlueprintType)
struct FZoneSaveData {
  GENERATED_BODY()

  // "������" ���� � ������ DataAsset, ����� ���������� ���� ��������
  UPROPERTY()
  FSoftObjectPath ZoneDataPath;

  // �������, ������� � ������� ���� � ����
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