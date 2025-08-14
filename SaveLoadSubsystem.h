// SaveLoadSubsystem.h

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveLoadSubsystem.generated.h"

UCLASS()
class SHOPPINGSIM_API USaveLoadSubsystem : public UGameInstanceSubsystem {
  GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable, Category = "SaveLoad")
  void SaveGame();

  UFUNCTION(BlueprintCallable, Category = "SaveLoad")
  void LoadGame();

private:
  FString SaveSlotName = TEXT("DefaultSaveSlot");
};