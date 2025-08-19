#pragma once


#include "CoreMinimal.h"
#include "Actors/InteractableActor.h"
#include "Utils/FurnitureTypes.h"
#include "StoreZoneActor.generated.h"

class UStoreZoneData;
class UInventoryComponent;
class UMaterialInstanceDynamic;
class USceneComponent;

UENUM(BlueprintType)
enum class EZoneStatus : uint8 { Active, Broken };

UCLASS()
class SHOPPINGSIM_API AStoreZoneActor : public AInteractableActor {
  GENERATED_BODY()

public:
  AStoreZoneActor();

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
  TObjectPtr<UStoreZoneData> ZoneData;

  UFUNCTION(BlueprintCallable, Category = "Inventory")
  UInventoryComponent *GetInventoryComponent() const;

  void BreakDown();
  void Repair();
  bool IsBroken() const;
  FText GetZoneName() const;

  virtual void OnInteract(AController *Interactor) override;
  virtual void PostInitializeComponents() override;

  // Методы работы со слотами
  bool IsValidSlot(int32 SlotIndex) const;
  bool IsSlotOccupied(int32 SlotIndex) const;
  bool CanPlaceFurnitureAtSlot(EFurnitureType Type, int32 SlotIndex) const;
  FVector GetSlotPosition(int32 SlotIndex) const;
  void SetSlotOccupied(int32 SlotIndex, bool bOccupied, AActor *What);
  EZoneType GetZoneType() const;

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  TObjectPtr<USceneComponent> MyRootComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  TObjectPtr<UStaticMeshComponent> MeshComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  TObjectPtr<UInventoryComponent> InventoryComponent;

private:
  UPROPERTY(VisibleAnywhere, Category = "State")
  EZoneStatus CurrentStatus = EZoneStatus::Active;

  UPROPERTY()
  TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;

  void PlayHitEffect(const FLinearColor &EmissiveColor);
};
