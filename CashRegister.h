// CashRegister.h

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "CashRegister.generated.h" // Этот include обязателен для UHT!

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
