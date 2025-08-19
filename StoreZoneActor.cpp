#include "Actors/StoreZoneActor.h"
#include "Actors/PlaceableActor.h"
#include "Components/InventoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Data/StoreZoneData.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Subsystems/EconomySubsystem.h"
#include "TimerManager.h"

AStoreZoneActor::AStoreZoneActor() {
  PrimaryActorTick.bCanEverTick = false;

  MyRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
  RootComponent = MyRootComponent;

  MeshComponent =
      CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
  MeshComponent->SetupAttachment(MyRootComponent);

  InventoryComponent =
      CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void AStoreZoneActor::PostInitializeComponents() {
  Super::PostInitializeComponents();

  if (!ZoneData)
    return;

  if (!ZoneData->ZoneMesh.IsNull()) {
    MeshComponent->SetStaticMesh(ZoneData->ZoneMesh.LoadSynchronous());
    DynamicMaterialInstance =
        MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
  }

  if (ZoneData->ZoneType == EZoneType::Template) {
    for (const FSubItemInfo &SubItemInfo : ZoneData->TemplateItems) {
      if (SubItemInfo.SubItemData) {
        AStoreZoneActor *SubItemActor = GetWorld()->SpawnActor<AStoreZoneActor>(
            AStoreZoneActor::StaticClass(), GetActorLocation(),
            GetActorRotation());

        if (SubItemActor) {
          SubItemActor->AttachToActor(
              this, FAttachmentTransformRules::KeepRelativeTransform);
          SubItemActor->SetActorRelativeTransform(
              SubItemInfo.RelativeTransform);
          SubItemActor->ZoneData = SubItemInfo.SubItemData;
          SubItemActor->PostInitializeComponents();
        }
      }
    }
  }
}

UInventoryComponent *AStoreZoneActor::GetInventoryComponent() const {
  return InventoryComponent ? InventoryComponent : nullptr;
}

void AStoreZoneActor::PlayHitEffect(const FLinearColor &EmissiveColor) {
  if (!DynamicMaterialInstance)
    return;

  DynamicMaterialInstance->SetVectorParameterValue(FName("EmissiveColor"),
                                                   EmissiveColor);

  FTimerHandle TimerHandle;
  GetWorld()->GetTimerManager().SetTimer(
      TimerHandle,
      [this]() {
        if (DynamicMaterialInstance)
          DynamicMaterialInstance->SetVectorParameterValue(
              FName("EmissiveColor"), FLinearColor::Black);
      },
      0.3f, false);
}

void AStoreZoneActor::BreakDown() {
  if (CurrentStatus == EZoneStatus::Active) {
    CurrentStatus = EZoneStatus::Broken;
    InteractionPromptText = FString(TEXT("REPAIR"));
    PlayHitEffect(FLinearColor::Red);
  }
}

void AStoreZoneActor::Repair() {
  if (CurrentStatus == EZoneStatus::Broken) {
    const int32 RepairCost = 50;
    if (UEconomySubsystem *Economy =
            GetWorld()->GetGameInstance()->GetSubsystem<UEconomySubsystem>()) {
      if (Economy->TrySpendMoney(RepairCost)) {
        CurrentStatus = EZoneStatus::Active;
        InteractionPromptText = FString(TEXT(""));
        PlayHitEffect(FLinearColor::Green);
        UE_LOG(LogTemp, Log, TEXT("Repaired %s for %d"),
               *GetZoneName().ToString(), RepairCost);
      } else {
        UE_LOG(LogTemp, Warning, TEXT("Not enough money to repair %s"),
               *GetZoneName().ToString());
      }
    }
  }
}

bool AStoreZoneActor::IsBroken() const {
  return CurrentStatus == EZoneStatus::Broken;
}

FText AStoreZoneActor::GetZoneName() const {
  return ZoneData ? ZoneData->ZoneName : FText::FromString("Unknown Zone");
}

void AStoreZoneActor::OnInteract(AController *Interactor) {
  Super::OnInteract(Interactor);

  if (IsBroken()) {
    Repair();
  }
}

// --- Методы для работы со слотами ---

bool AStoreZoneActor::IsValidSlot(int32 SlotIndex) const {
  return ZoneData && SlotIndex >= 0 &&
         ZoneData->ZoneSlots.IsValidIndex(SlotIndex);
}

bool AStoreZoneActor::IsSlotOccupied(int32 SlotIndex) const {
  return IsValidSlot(SlotIndex) ? ZoneData->ZoneSlots[SlotIndex].bOccupied
                                : false;
}

bool AStoreZoneActor::CanPlaceFurnitureAtSlot(EFurnitureType Type,
                                              int32 SlotIndex) const {
  return IsValidSlot(SlotIndex) &&
         ZoneData->ZoneSlots[SlotIndex].AllowedFurnitureType == Type &&
         !ZoneData->ZoneSlots[SlotIndex].bOccupied;
}

FVector AStoreZoneActor::GetSlotPosition(int32 SlotIndex) const {
  return IsValidSlot(SlotIndex) ? ZoneData->ZoneSlots[SlotIndex].LocalPosition
                                : FVector::ZeroVector;
}

void AStoreZoneActor::SetSlotOccupied(int32 SlotIndex, bool bOccupied,
                                      AActor * /*What*/) {
  if (IsValidSlot(SlotIndex))
    ZoneData->ZoneSlots[SlotIndex].bOccupied = bOccupied;
}

EZoneType AStoreZoneActor::GetZoneType() const {
  return ZoneData ? ZoneData->ZoneType : EZoneType::Custom;
}
