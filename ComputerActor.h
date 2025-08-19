#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ComputerActor.generated.h"

class UWidgetComponent;
class UStaticMeshComponent;

UCLASS()
class SHOPPINGSIM_API AComputerActor : public AActor {
  GENERATED_BODY()

public:
  AComputerActor();

  // Сделайте публичным, чтобы можно было обращаться извне
  UPROPERTY(VisibleAnywhere)
  UWidgetComponent *DesktopWidgetComp;

protected:
  virtual void BeginPlay() override; // <--- Добавьте это объявление

  UPROPERTY(VisibleAnywhere)
  UStaticMeshComponent *MonitorMesh;
};
