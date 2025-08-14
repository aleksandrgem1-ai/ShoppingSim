// BuildManagerSubsystem.h (ПОЛНАЯ ВЕРСИЯ)

#pragma once


#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BuildManagerSubsystem.generated.h"

class UStoreZoneData;
class AStoreZoneActor;
class UInputMappingContext;

UCLASS()
class SHOPPINGSIM_API UBuildManagerSubsystem : public UGameInstanceSubsystem {
  GENERATED_BODY()

public:
  virtual void Initialize(FSubsystemCollectionBase &Collection) override;

  // --- ЕДИНАЯ ТОЧКА ВХОДА ДЛЯ ПЕРЕКЛЮЧЕНИЯ ---
  void ToggleBuildMode();

  // --- Функции для управления процессом строительства ---
  void StartPlacement();
  void UpdateGhostActorTransform();
  void ConfirmPlacement();
  void CancelPlacement();

  void SelectObjectForMove(AStoreZoneActor *ZoneToMove);
  void HandleDestruction(AStoreZoneActor *ZoneToDestroy);

  void ToggleGridSnapping();
  void AddRotation(float Angle);

private:
  // --- Приватные функции для внутренней логики ---
  void EnterBuildMode();
  void ExitBuildMode();

  // --- Свойства для переключения режимов ---
  UPROPERTY() TObjectPtr<APawn> OriginalPlayerPawn;
  UPROPERTY() TObjectPtr<UInputMappingContext> PlayerMappingContext;
  UPROPERTY() TObjectPtr<UInputMappingContext> BuildModeMappingContext;
  TSubclassOf<APawn> BuildModePawnClass;

  // --- Свойства для управления строительством ---
  UPROPERTY() TObjectPtr<UStoreZoneData> TestZoneToBuild;
  UPROPERTY() TObjectPtr<AStoreZoneActor> GhostActor;

  // --- ГЛАВНЫЙ ФЛАГ СОСТОЯНИЯ ---
  bool bIsInBuildMode = false;

  bool bIsGridSnappingEnabled = true;
  float CurrentPlacementRotation = 0.0f;
};