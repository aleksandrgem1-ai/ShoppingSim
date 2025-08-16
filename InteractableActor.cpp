// InteractableActor.cpp
#include "InteractableActor.h"

AInteractableActor::AInteractableActor() {
  PrimaryActorTick.bCanEverTick = false;

  // Инициализируем корневой компонент
  Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  RootComponent = Root;

  InteractionPromptText =
      FString(TEXT("To interact")); // Текст по умолчанию
}

// Базовая реализация. В дочерних классах мы будем её переопределять.
void AInteractableActor::OnInteract(AController *Interactor) {
  UE_LOG(LogTemp, Log, TEXT("'%s' has been used!"), *GetName());
}