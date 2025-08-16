// EconomySubsystem.h

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EconomySubsystem.generated.h"

// Баланс
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBalanceChangedSignature, int32,
                                            NewBalance);

// Доход (за текущий день/период)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIncomeChangedSignature, int32,
                                            NewIncome);

UCLASS()
class SHOPPINGSIM_API UEconomySubsystem : public UGameInstanceSubsystem {
  GENERATED_BODY()

public:
  // Начислить деньги (увеличивает баланс и доход)
  UFUNCTION(BlueprintCallable, Category = "Economy")
  void AddMoney(int32 Amount);

  // Потратить деньги. Возвращает true, если успешно
  UFUNCTION(BlueprintCallable, Category = "Economy")
  bool TrySpendMoney(int32 Amount);

  // Получить текущий баланс
  UFUNCTION(BlueprintPure, Category = "Economy")
  int32 GetCurrentBalance() const;

  // Получить текущий доход за период
  UFUNCTION(BlueprintPure, Category = "Economy")
  int32 GetCurrentIncome() const;

  // Сбросить доход (например, в начале нового дня)
  UFUNCTION(BlueprintCallable, Category = "Economy")
  void ResetIncome();

  // Делегаты для UI
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
