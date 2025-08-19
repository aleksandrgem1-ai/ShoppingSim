// BuildManagerSubsystem.h (fixed)
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BuildManagerSubsystem.generated.h"

class UStoreZoneData;
class AStoreZoneActor;
class UInputMappingContext;
class ABuildModePawn;
class APawn;

UCLASS()
class SHOPPINGSIM_API UBuildManagerSubsystem : public UGameInstanceSubsystem {
  GENERATED_BODY()

public:
  virtual void Initialize(FSubsystemCollectionBase &Collection) override;

  // ЕДИНАЯ ТОЧКА ВХОДА
  void ToggleBuildMode();

  // Управление строительством
  void StartPlacement();
  void UpdateGhostActorTransform();
  void ConfirmPlacement();
  void CancelPlacement();
  void SelectObjectForMove(AStoreZoneActor *ZoneToMove);
  void HandleDestruction(AStoreZoneActor *ZoneToDestroy);
  void ToggleGridSnapping();
  void AddRotation(float Angle);

private:
  // Внутренняя логика
  void EnterBuildMode();
  void ExitBuildMode();

  // Режимы/инпут
  UPROPERTY() TObjectPtr<APawn> OriginalPlayerPawn = nullptr;
  UPROPERTY() TObjectPtr<UInputMappingContext> PlayerMappingContext = nullptr;
  UPROPERTY()
  TObjectPtr<UInputMappingContext> BuildModeMappingContext = nullptr;
  UPROPERTY() TSubclassOf<ABuildModePawn> BuildModePawnClass;
  UPROPERTY(Transient) TObjectPtr<APawn> SpawnedBuildPawn = nullptr;

  // Строительство
  UPROPERTY() TObjectPtr<UStoreZoneData> TestZoneToBuild = nullptr;
  UPROPERTY() TObjectPtr<AStoreZoneActor> GhostActor = nullptr;

  // Состояние
  bool bIsInBuildMode = false;
  bool bIsGridSnappingEnabled = true;
  float CurrentPlacementRotation = 0.0f;
};