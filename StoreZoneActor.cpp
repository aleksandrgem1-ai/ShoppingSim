// StoreZoneActor.cpp (Замена для PlaceableActor.cpp)

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

  // Устанавливаем меш из Data Asset, если он назначен
  if (ZoneData && !ZoneData->ZoneMesh.IsNull()) {
    MeshComponent->SetStaticMesh(ZoneData->ZoneMesh.LoadSynchronous());
  }
}