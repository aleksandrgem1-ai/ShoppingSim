// PlaceableActor.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaceableActor.generated.h"

UCLASS()
class SHOPPINGSIM_API APlaceableActor : public AActor {
  GENERATED_BODY()

public:
  APlaceableActor();

  /** ��������� ���������� ����� ������� */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Economy")
  int32 Cost = 100;

  /** �����, ������� ���� ������ ���������� � ������� */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Economy")
  float IncomePerSecond = 1.0f;

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TObjectPtr<UStaticMeshComponent> Mesh;
};