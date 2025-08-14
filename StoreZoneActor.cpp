// StoreZoneActor.cpp (ОБНОВЛЕННЫЙ)

#include "StoreZoneActor.h"
#include "Components/StaticMeshComponent.h"
#include "InventoryComponent.h" // <-- Инклюд нового компонента
#include "StoreZoneData.h"

AStoreZoneActor::AStoreZoneActor() {
  PrimaryActorTick.bCanEverTick = false;

  MeshComponent =
      CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
  RootComponent = MeshComponent;

  // --- НОВОЕ: Создаем компонент инвентаря ---
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