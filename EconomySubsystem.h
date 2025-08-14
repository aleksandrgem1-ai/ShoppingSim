// EconomySubsystem.h

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EconomySubsystem.generated.h"

// ��������� �������, ������� ����� ��������� �� ��������� �������.
// �� ����� �������� � Blueprint.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBalanceChangedSignature, int32,
                                            NewBalance);

UCLASS()
class SHOPPINGSIM_API UEconomySubsystem : public UGameInstanceSubsystem {
  GENERATED_BODY()

public:
  // ��������� ������ �� ����
  UFUNCTION(BlueprintCallable, Category = "Economy")
  void AddMoney(int32 Amount);

  // �������� ��������� ������. ���������� true, ���� �������.
  UFUNCTION(BlueprintCallable, Category = "Economy")
  bool TrySpendMoney(int32 Amount);

  // ���������� ������� ������
  UFUNCTION(BlueprintPure, Category = "Economy")
  int32 GetCurrentBalance() const;

  // �������, �� ������� ����� ����������� ��� ���������� UI
  UPROPERTY(BlueprintAssignable, Category = "Economy")
  FOnBalanceChangedSignature OnBalanceChanged;

private:
  UPROPERTY()
  int32 CurrentBalance = 0;
};