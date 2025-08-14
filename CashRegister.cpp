// CashRegister.cpp

#include "CashRegister.h"
#include "Components/StaticMeshComponent.h"

ACashRegister::ACashRegister() {
  // �������������, ��� Tick() ��� �� ����� ��� ����� �������
  PrimaryActorTick.bCanEverTick = false;

  MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
  RootComponent = MeshComponent;

  InteractionPromptText = FString(TEXT("Open CashMachine"));
}

void ACashRegister::OnInteract(AController *Interactor) {
  // �������� ������������ ���������� �� ������, ���� ��� ���� ������ ������
  Super::OnInteract(Interactor);

  // ������� ���� ���������� ��������� � ���
  UE_LOG(LogTemp, Warning, TEXT("KA-CHING! CashMachine used player %s."),
         *GetNameSafe(Interactor));
}