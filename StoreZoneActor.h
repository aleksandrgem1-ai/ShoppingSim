// StoreZoneActor.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StoreZoneActor.generated.h" // Эта строка уже будет здесь

UCLASS()
class SHOPPINGSIM_API AStoreZoneActor : public AActor {
  GENERATED_BODY()

public:
  AStoreZoneActor();

  // --- НАЧАЛО ВСТАВЛЕННОГО КОДА ИЗ PLACEABLEACTOR.H ---
  /** Стоимость размещения этого объекта */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Economy")
  int32 Cost = 100;

  /** Доход, который этот объект генерирует в секунду */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Economy")
  float IncomePerSecond = 1.0f;

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UStaticMeshComponent> Mesh;
  // --- КОНЕЦ ВСТАВЛЕННОГО КОДА ---
};