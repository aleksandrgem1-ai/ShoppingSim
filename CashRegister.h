// CashRegister.h 

#pragma once

#include "CoreMinimal.h"
#include "Actors/InteractableActor.h"
#include "CashRegister.generated.h" // ���� include ���������� ��� UHT!

UCLASS()
class SHOPPINGSIM_API ACashRegister : public AInteractableActor
{
    GENERATED_BODY()

public:
    ACashRegister();

    virtual void OnInteract(AController* Interactor) override;

private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> MeshComponent;
};
