// BuildManagerSubsystem.h(�����������)

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
  // ����������� ������� ������������� ��� ���������
  virtual void Initialize(FSubsystemCollectionBase &Collection) override;

  // ���� � ����� �������������
  UFUNCTION(BlueprintCallable, Category = "Building")
  void EnterBuildMode();

  // ����� �� ������ �������������
  UFUNCTION(BlueprintCallable, Category = "Building")
  void ExitBuildMode();

  // ... ��������� ��� ...

private:
  // ��������� ��������� �� ������������� ���������
  UPROPERTY()
  TObjectPtr<APawn> OriginalPlayerPawn;

  // ������ IMC (������ ����������� � .cpp)
  UPROPERTY()
  TObjectPtr<UInputMappingContext> PlayerMappingContext;

  UPROPERTY()
  TObjectPtr<UInputMappingContext> BuildModeMappingContext;

  // ����� ��� ������ (������ ��������������� � .cpp)
  TSubclassOf<APawn> BuildModePawnClass;
};