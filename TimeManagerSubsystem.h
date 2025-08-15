// ShoppingSim/TimeManagerSubsystem.h

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TimeManagerSubsystem.generated.h"

// Forward declare classes
class UCurveFloat;

// Делегат для обновления времени (часы и минуты)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimeUpdated, int32, Hour, int32,
                                             Minute);
// Делегат для смены дня (со всей нужной информацией)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDayChanged, int32, NewDay,
                                              int32, PreviousDayIncome, int32,
                                              PreviousDayGoal, int32,
                                              NewDayGoal);

/**
 * Manages game time, day/night cycle, and triggers demand calculations.
 */
UCLASS()
class SHOPPINGSIM_API UTimeManagerSubsystem : public UWorldSubsystem {
  GENERATED_BODY()

public:
  // Lifecycle functions
  virtual void Initialize(FSubsystemCollectionBase &Collection) override;
  virtual void Deinitialize() override;

  // Function to get current time information for compatibility
  UFUNCTION(BlueprintPure, Category = "Time")
  void GetCurrentDayInfo(int &OutDay, int &OutHour) const;

  // Делегаты для UI
  UPROPERTY(BlueprintAssignable, Category = "Time")
  FOnTimeUpdated OnTimeUpdated;

  UPROPERTY(BlueprintAssignable, Category = "Time")
  FOnDayChanged OnDayChanged;

private:
  // Переменные времени и экономики
  UPROPERTY()
  float DayDurationInSeconds = 1200.0f;

  UPROPERTY()
  float TimeElapsedInDay = 0.0f;

  UPROPERTY()
  int32 CurrentDay = 1;

  UPROPERTY()
  int32 CurrentDayIncome = 0;

  UPROPERTY()
  int32 CurrentDayGoal = 1000;

  UPROPERTY()
  TObjectPtr<UCurveFloat> DemandCurve;

  FTimerHandle SecondTickTimerHandle;

  // Приватные функции
  void TickSecond();
};