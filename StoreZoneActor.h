// StoreZoneActor.h (������������ ������)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StoreZoneActor.generated.h"

class UStoreZoneData;

UCLASS()
class SHOPPINGSIM_API AStoreZoneActor : public AActor {
  GENERATED_BODY()

public: // <-- ��������� ����� (���� protected)
  AStoreZoneActor();

  // ������, ������� ���������� ��� ����
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
  TObjectPtr<UStoreZoneData> ZoneData;

  // ���������� ����� ����, ��� ��� ���������� ����������������
  virtual void PostInitializeComponents() override;

protected: // <-- ���������� ����� �������� �����
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  TObjectPtr<UStaticMeshComponent> MeshComponent;
};