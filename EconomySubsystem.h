// EconomySubsystem.h

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EconomySubsystem.generated.h"

// ������
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBalanceChangedSignature, int32,
                                            NewBalance);

// ����� (�� ������� ����/������)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIncomeChangedSignature, int32,
                                            NewIncome);

UCLASS()
class SHOPPINGSIM_API UEconomySubsystem : public UGameInstanceSubsystem {
  GENERATED_BODY()

public:
  // ��������� ������ (����������� ������ � �����)
  UFUNCTION(BlueprintCallable, Category = "Economy")
  void AddMoney(int32 Amount);

  // ��������� ������. ���������� true, ���� �������
  UFUNCTION(BlueprintCallable, Category = "Economy")
  bool TrySpendMoney(int32 Amount);

  // �������� ������� ������
  UFUNCTION(BlueprintPure, Category = "Economy")
  int32 GetCurrentBalance() const;

  // �������� ������� ����� �� ������
  UFUNCTION(BlueprintPure, Category = "Economy")
  int32 GetCurrentIncome() const;

  // �������� ����� (��������, � ������ ������ ���)
  UFUNCTION(BlueprintCallable, Category = "Economy")
  void ResetIncome();

  // �������� ��� UI
  UPROPERTY(BlueprintAssignable, Category = "Economy")
  FOnBalanceChangedSignature OnBalanceChanged;

  UPROPERTY(BlueprintAssignable, Category = "Economy")
  FOnIncomeChangedSignature OnIncomeChanged;

private:
  UPROPERTY()
  int32 CurrentBalance = 0;

  UPROPERTY()
  int32 CurrentIncome = 0;
};
