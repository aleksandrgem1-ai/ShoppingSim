// CashRegister.cpp

#include "CashRegister.h"
#include "Components/StaticMeshComponent.h"

ACashRegister::ACashRegister() {
  // Устанавливаем, что Tick() нам не нужен для этого объекта
  PrimaryActorTick.bCanEverTick = false;

  MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
  RootComponent = MeshComponent;

  InteractionPromptText = FString(TEXT("Open CashMachine"));
}

void ACashRegister::OnInteract(AController *Interactor) {
  // Вызываем родительскую реализацию на случай, если там есть важная логика
  Super::OnInteract(Interactor);

  // Выводим наше уникальное сообщение в лог
  UE_LOG(LogTemp, Warning, TEXT("KA-CHING! CashMachine used player %s."),
         *GetNameSafe(Interactor));
}