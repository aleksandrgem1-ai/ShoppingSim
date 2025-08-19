// InteractableActor.cpp 
#include "Actors/InteractableActor.h"

AInteractableActor::AInteractableActor() {
  PrimaryActorTick.bCanEverTick = false;

  // �������������� �������� ���������
  Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
  RootComponent = Root;

  InteractionPromptText =
      FString(TEXT("To interact")); // ����� �� ���������
}

// ������� ����������. � �������� ������� �� ����� � ��������������.
void AInteractableActor::OnInteract(AController *Interactor) {
  UE_LOG(LogTemp, Log, TEXT("'%s' has been used!"), *GetName());
}