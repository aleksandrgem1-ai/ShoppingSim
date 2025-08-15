// SaveLoadSubsystem.h (»—œ–¿¬À≈ÕÕ€…)

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveLoadSubsystem.generated.h"

UCLASS()
class SHOPPINGSIM_API USaveLoadSubsystem : public UGameInstanceSubsystem {
  GENERATED_BODY()

public:
  virtual void Initialize(FSubsystemCollectionBase &Collection) override;

  UFUNCTION(BlueprintCallable, Category = "SaveLoad")
  void SaveGame();

  UFUNCTION(BlueprintCallable,
            Category = "SaveLoad") // <-- »—œ–¿¬À≈Õ¿ Œœ≈◊¿“ ¿ «ƒ≈—‹
  void LoadGame();

private:
  FString SaveSlotName = TEXT("DefaultSaveSlot");
  FTimerHandle AutosaveTimerHandle;
};