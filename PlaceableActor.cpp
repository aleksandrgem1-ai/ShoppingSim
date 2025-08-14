// PlaceableActor.cpp
#include "PlaceableActor.h"

APlaceableActor::APlaceableActor() {
  PrimaryActorTick.bCanEverTick = false;

  Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
  RootComponent = Mesh;
}