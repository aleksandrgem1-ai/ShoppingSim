// StoreZoneActor.cpp (онкмюъ бепяхъ)

#include "StoreZoneActor.h"
#include "Components/StaticMeshComponent.h"
#include "EconomySubsystem.h"
#include "InventoryComponent.h"
#include "StoreZoneData.h"

AStoreZoneActor::AStoreZoneActor() {
  PrimaryActorTick.bCanEverTick = false;

  MeshComponent =
      CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
  if (RootComponent) {
    MeshComponent->SetupAttachment(RootComponent);
  }

  InventoryComponent =
      CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void AStoreZoneActor::PostInitializeComponents() {
  Super::PostInitializeComponents();

  if (!ZoneData)
    return;

  if (!ZoneData->ZoneMesh.IsNull()) {
    MeshComponent->SetStaticMesh(ZoneData->ZoneMesh.LoadSynchronous());
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

void AStoreZoneActor::BreakDown() {
  if (CurrentStatus == EZoneStatus::Active) {
    CurrentStatus = EZoneStatus::Broken;
    InteractionPromptText = FString(TEXT("REPAIR"));
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