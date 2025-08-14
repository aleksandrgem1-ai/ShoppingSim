// StoreZoneActor.h (�����������)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StoreZoneActor.generated.h"

class UStoreZoneData;
class UInventoryComponent; // <-- Forward declaration

UCLASS()
class SHOPPINGSIM_API AStoreZoneActor : public AActor {
  GENERATED_BODY()

public:
  AStoreZoneActor();

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
  TObjectPtr<UStoreZoneData> ZoneData;

  // --- �����: ��������� ������ � ��������� ---
  UInventoryComponent *GetInventoryComponent() const {
    return InventoryComponent;
  }

public:
  virtual void PostInitializeComponents() override;

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  TObjectPtr<UStaticMeshComponent> MeshComponent;

  // --- �����: ��������� ��������� ---
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  TObjectPtr<UInventoryComponent> InventoryComponent;
};