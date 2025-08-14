// EconomySubsystem.cpp

#include "EconomySubsystem.h"

void UEconomySubsystem::AddMoney(int32 Amount) {
  if (Amount > 0) {
    CurrentBalance += Amount;
    // Оповещаем всех подписчиков об изменении баланса
    OnBalanceChanged.Broadcast(CurrentBalance);
  }
}

bool UEconomySubsystem::TrySpendMoney(int32 Amount) {
  if (Amount > 0 && CurrentBalance >= Amount) {
    CurrentBalance -= Amount;
    // Оповещаем всех подписчиков об изменении баланса
    OnBalanceChanged.Broadcast(CurrentBalance);
    return true;
  }
  return false;
}

int32 UEconomySubsystem::GetCurrentBalance() const { return CurrentBalance; }