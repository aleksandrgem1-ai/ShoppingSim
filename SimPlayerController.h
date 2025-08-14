// SimPlayerController.h (������������ ������)
// �� ������� ��� ���������� CurrentMoney, AddMoney, Load... � OnPossess,
// ��� ��� ���� ������ � ����������� ������ ���.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SimPlayerController.generated.h"

class UInputMappingContext;
class UMoneyHUDWidget; // forward declaration

UCLASS()
class SHOPPINGSIM_API ASimPlayerController : public APlayerController {
  GENERATED_BODY()

public:
  ASimPlayerController();

protected:
  // BeginPlay - ��� ������������ ������� ���������� �����, ������� ��� �����
  // �����
  virtual void BeginPlay() override;

private:
  // IMC ����������� �� �������� � ������������
  UPROPERTY()
  TObjectPtr<UInputMappingContext> DefaultMappingContext = nullptr;

  UPROPERTY()
  int32 MappingPriority = 0;

  void AddDefaultIMC();

  // ��� �������� ������ ����� ��������� �� �������� ������� � .cpp �����
  TSubclassOf<UMoneyHUDWidget> MoneyHUDClass;

  // ��������� �� ��������� ������ ��� HUD
  UPROPERTY(Transient)
  TObjectPtr<UMoneyHUDWidget> MoneyHUD = nullptr;
};