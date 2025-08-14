// EconomySubsystem.h

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EconomySubsystem.generated.h"

// Объявляем делегат, который будет оповещать об изменении баланса.
// Он будет доступен в Blueprint.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBalanceChangedSignature, int32,
                                            NewBalance);

UCLASS()
class SHOPPINGSIM_API UEconomySubsystem : public UGameInstanceSubsystem {
  GENERATED_BODY()

public:
  // Добавляет деньги на счет
  UFUNCTION(BlueprintCallable, Category = "Economy")
  void AddMoney(int32 Amount);

  // Пытается потратить деньги. Возвращает true, если успешно.
  UFUNCTION(BlueprintCallable, Category = "Economy")
  bool TrySpendMoney(int32 Amount);

  // Возвращает текущий баланс
  UFUNCTION(BlueprintPure, Category = "Economy")
  int32 GetCurrentBalance() const;

  // Делегат, на который можно подписаться для обновления UI
  UPROPERTY(BlueprintAssignable, Category = "Economy")
  FOnBalanceChangedSignature OnBalanceChanged;

private:
  UPROPERTY()
  int32 CurrentBalance = 0;
};