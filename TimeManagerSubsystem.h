// TimeManagerSubsystem.h 

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TimeManagerSubsystem.generated.h"

class UCurveFloat;

// --- ОБЪЯВЛЯЕМ НОВЫЕ ДЕЛЕГАТЫ ---
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimeUpdatedSignature, int32,
                                             Hour, int32, Minute);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDayChangedSignature, int32,
                                              NewDay, int32,
                                              IncomeForPreviousDay, int32,
                                              GoalForPreviousDay, int32,
                                              NewGoal);

UCLASS()
class SHOPPINGSIM_API UTimeManagerSubsystem : public UWorldSubsystem {
  GENERATED_BODY()

public:
  virtual void Initialize(FSubsystemCollectionBase &Collection) override;
  virtual void Deinitialize() override;

  UPROPERTY(BlueprintAssignable, Category = "Time")
  FOnDayChangedSignature OnDayChanged;

  // --- ОБЪЯВЛЯЕМ НОВЫЙ ДЕЛЕГАТ ---
  UPROPERTY(BlueprintAssignable, Category = "Time")
  FOnTimeUpdatedSignature OnTimeUpdated;

  // --- ОБЪЯВЛЯЕМ НОВУЮ ФУНКЦИЮ ---
  void GetCurrentDayInfo(int32 &OutDay, int32 &OutGoal) const;

private:
  void TickSecond();

  UPROPERTY()
  TObjectPtr<UCurveFloat> DemandCurve;

  float DayDurationInSeconds = 120.0f;

  int32 CurrentDay = 1;
  float TimeElapsedInDay = 0.0f;
  int32 CurrentDayIncome = 0;
  int32 CurrentDayGoal = 100;

  FTimerHandle SecondTickTimerHandle;
};
