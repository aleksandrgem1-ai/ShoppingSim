// StoreZoneActor.h (ИСПРАВЛЕННЫЙ)

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "StoreZoneActor.generated.h"

class UStoreZoneData;
class UInventoryComponent;

// Состояния объекта
UENUM(BlueprintType)
enum class EZoneStatus : uint8 { Active, Broken };

UCLASS()
class SHOPPINGSIM_API AStoreZoneActor : public AInteractableActor {
  GENERATED_BODY()

public:
  AStoreZoneActor();

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
  TObjectPtr<UStoreZoneData> ZoneData;

  UInventoryComponent *GetInventoryComponent() const {
    return InventoryComponent;
  }

  // --- Новые функции для управления состоянием ---
  void BreakDown();
  void Repair();
  bool IsBroken() const;
  FText GetZoneName() const;

  // Переопределяем функцию взаимодействия из родительского класса
  virtual void OnInteract(AController *Interactor) override;

public:
  virtual void PostInitializeComponents() override;

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  TObjectPtr<UStaticMeshComponent> MeshComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  TObjectPtr<UInventoryComponent> InventoryComponent;

private:
  UPROPERTY(VisibleAnywhere, Category = "State")
  EZoneStatus CurrentStatus = EZoneStatus::Active;
};