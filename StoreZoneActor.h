// StoreZoneActor.h (ИСПРАВЛЕННАЯ ВЕРСИЯ)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StoreZoneActor.generated.h"

class UStoreZoneData;

UCLASS()
class SHOPPINGSIM_API AStoreZoneActor : public AActor {
  GENERATED_BODY()

public: // <-- ИЗМЕНЕНИЕ ЗДЕСЬ (было protected)
  AStoreZoneActor();

  // Данные, которые определяют эту зону
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
  TObjectPtr<UStoreZoneData> ZoneData;

  // Вызывается после того, как все компоненты инициализированы
  virtual void PostInitializeComponents() override;

protected: // <-- Компоненты могут остаться здесь
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  TObjectPtr<UStaticMeshComponent> MeshComponent;
};