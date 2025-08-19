#include "Actors/ComputerActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"

AComputerActor::AComputerActor() {
  PrimaryActorTick.bCanEverTick = false;

  MonitorMesh =
      CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MonitorMesh"));
  RootComponent = MonitorMesh;

  DesktopWidgetComp =
      CreateDefaultSubobject<UWidgetComponent>(TEXT("DesktopWidgetComp"));
  DesktopWidgetComp->SetupAttachment(MonitorMesh);
  DesktopWidgetComp->SetDrawSize(FVector2D(1024, 768));   // Размер UI
  DesktopWidgetComp->SetWidgetSpace(EWidgetSpace::World); // 3D-встраивание
  DesktopWidgetComp->SetRelativeLocation(
      FVector(0, 0, 30)); // подстройте под вашу модель

  DesktopWidgetComp->SetVisibility(false); // Скрываем UI изначально
}

void AComputerActor::BeginPlay() {
  Super::BeginPlay();
  // Можно добавить код инициализации, если нужно
}
