// InteractableActor.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class SHOPPINGSIM_API AInteractableActor : public AActor {
  GENERATED_BODY()

public:
  AInteractableActor();

  /** ��� ������� ����� ����������, ����� ����� ��������������� � �������� */
  UFUNCTION(BlueprintCallable, Category = "Interaction")
  virtual void OnInteract(AController *Interactor);

  /** �����, ������� ������ �����, ��������� �� ������ */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
  FString InteractionPromptText;

protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  USceneComponent *Root; // ��������� �������� ��������� ��� ������������
};