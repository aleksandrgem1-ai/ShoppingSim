// StoreZoneActor.cpp (ОБНОВЛЕННЫЙ)

#include "StoreZoneActor.h"
#include "Components/StaticMeshComponent.h"
#include "StoreZoneData.h"

AStoreZoneActor::AStoreZoneActor() {
  PrimaryActorTick.bCanEverTick = false;

  MeshComponent =
      CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
  RootComponent = MeshComponent;
}

void AStoreZoneActor::PostInitializeComponents() {
  Super::PostInitializeComponents();

  if (!ZoneData)
    return;

  // 1. Устанавливаем основной меш (для Prop это будет сама модель, для Template
  // - пол)
  if (!ZoneData->ZoneMesh.IsNull()) {
    MeshComponent->SetStaticMesh(ZoneData->ZoneMesh.LoadSynchronous());
  }

  // 2. Если это шаблон, создаем все дочерние объекты
  if (ZoneData->ZoneType == EZoneType::Template) {
    for (const FSubItemInfo &SubItemInfo : ZoneData->TemplateItems) {
      if (SubItemInfo.SubItemData) {
        // Создаем новый дочерний объект
        AStoreZoneActor *SubItemActor = GetWorld()->SpawnActor<AStoreZoneActor>(
            AStoreZoneActor::StaticClass(), GetActorLocation(),
            GetActorRotation());
        if (SubItemActor) {
          // Прикрепляем его к нашему шаблону
          SubItemActor->AttachToActor(
              this, FAttachmentTransformRules::KeepRelativeTransform);
          // Устанавливаем его позицию и поворот относительно шаблона
          SubItemActor->SetActorRelativeTransform(
              SubItemInfo.RelativeTransform);
          // Назначаем ему данные и инициализируем
          SubItemActor->ZoneData = SubItemInfo.SubItemData;
          SubItemActor->PostInitializeComponents();
        }
      }
    }
  }
}