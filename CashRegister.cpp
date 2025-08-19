// CashRegister.cpp (�����������) 

#include "Actors/CashRegister.h"
#include "Components/StaticMeshComponent.h"
#include "Subsystems/EconomySubsystem.h"

ACashRegister::ACashRegister() {
  PrimaryActorTick.bCanEverTick = false;

  MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
  RootComponent = MeshComponent;

  InteractionPromptText =
      FString(TEXT("Get Daily Profit"));
}

void ACashRegister::OnInteract(AController *Interactor) {
  Super::OnInteract(Interactor);

  if (UEconomySubsystem *EconomySubsystem =
          GetWorld()->GetGameInstance()->GetSubsystem<UEconomySubsystem>()) {
    const int32 ProfitAmount = 100; // ����� ��� �������
    EconomySubsystem->AddMoney(ProfitAmount);

    UE_LOG(LogTemp, Log, TEXT("Added %d to balance via Cash Register."),
           ProfitAmount);
  } else {
    UE_LOG(LogTemp, Error,
           TEXT("CashRegister could not find EconomySubsystem!"));
  }
}