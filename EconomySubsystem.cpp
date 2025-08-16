// EconomySubsystem.cpp

#include "EconomySubsystem.h"

void UEconomySubsystem::AddMoney(int32 Amount) {
  if (Amount > 0) {
    CurrentBalance += Amount;
    CurrentIncome += Amount; // фикс: учитываем доход

    OnBalanceChanged.Broadcast(CurrentBalance);
    OnIncomeChanged.Broadcast(CurrentIncome);

    UE_LOG(LogTemp, Verbose,
           TEXT("[ECONOMY] AddMoney: +%d, Balance=%d, Income=%d"), Amount,
           CurrentBalance, CurrentIncome);
  }
}

bool UEconomySubsystem::TrySpendMoney(int32 Amount) {
  if (Amount > 0 && CurrentBalance >= Amount) {
    CurrentBalance -= Amount;
    OnBalanceChanged.Broadcast(CurrentBalance);

    UE_LOG(LogTemp, Verbose, TEXT("[ECONOMY] SpendMoney: -%d, Balance=%d"),
           Amount, CurrentBalance);
    return true;
  }
  return false;
}

int32 UEconomySubsystem::GetCurrentBalance() const { return CurrentBalance; }

int32 UEconomySubsystem::GetCurrentIncome() const { return CurrentIncome; }

void UEconomySubsystem::ResetIncome() {
  CurrentIncome = 0;
  OnIncomeChanged.Broadcast(CurrentIncome);
  UE_LOG(LogTemp, Verbose, TEXT("[ECONOMY] Income reset to 0"));
}
