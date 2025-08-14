// BuildManagerSubsystem.h (������ ������)

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

  // --- ������ ����� ����� ��� ������������ ---
  void ToggleBuildMode();

  // --- ������� ��� ���������� ��������� ������������� ---
  void StartPlacement();
  void UpdateGhostActorTransform();
  void ConfirmPlacement();
  void CancelPlacement();

  void SelectObjectForMove(AStoreZoneActor *ZoneToMove);
  void HandleDestruction(AStoreZoneActor *ZoneToDestroy);

  void ToggleGridSnapping();
  void AddRotation(float Angle);

private:
  // --- ��������� ������� ��� ���������� ������ ---
  void EnterBuildMode();
  void ExitBuildMode();

  // --- �������� ��� ������������ ������� ---
  UPROPERTY() TObjectPtr<APawn> OriginalPlayerPawn;
  UPROPERTY() TObjectPtr<UInputMappingContext> PlayerMappingContext;
  UPROPERTY() TObjectPtr<UInputMappingContext> BuildModeMappingContext;
  TSubclassOf<APawn> BuildModePawnClass;

  // --- �������� ��� ���������� �������������� ---
  UPROPERTY() TObjectPtr<UStoreZoneData> TestZoneToBuild;
  UPROPERTY() TObjectPtr<AStoreZoneActor> GhostActor;

  // --- ������� ���� ��������� ---
  bool bIsInBuildMode = false;

  bool bIsGridSnappingEnabled = true;
  float CurrentPlacementRotation = 0.0f;
};