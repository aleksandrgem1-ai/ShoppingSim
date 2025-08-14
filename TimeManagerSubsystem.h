// TimeManagerSubsystem.h (НОВЫЙ ФАЙЛ)
// Наследовать от UWorldSubsystem

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TimeManagerSubsystem.generated.h"

// Делегат для оповещения UI о смене дня
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

  // Делегат, на который может подписаться UI
  UPROPERTY(BlueprintAssignable, Category = "Time")
  FOnDayChangedSignature OnDayChanged;

private:
  // Функция, которая будет вызываться каждую секунду
  void TickSecond();

  // --- Настройки игрового дня ---
  float DayDurationInSeconds = 120.0f; // 2 минуты

  // --- Состояние текущего дня ---
  int32 CurrentDay = 1;
  float TimeElapsedInDay = 0.0f;
  int32 CurrentDayIncome = 0;
  int32 CurrentDayGoal = 100; // Цель на первый день

  // Хэндл для нашего таймера
  FTimerHandle SecondTickTimerHandle;
};