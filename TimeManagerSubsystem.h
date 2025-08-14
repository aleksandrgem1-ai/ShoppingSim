// TimeManagerSubsystem.h (����� ����)
// ����������� �� UWorldSubsystem

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TimeManagerSubsystem.generated.h"

// ������� ��� ���������� UI � ����� ���
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

  // �������, �� ������� ����� ����������� UI
  UPROPERTY(BlueprintAssignable, Category = "Time")
  FOnDayChangedSignature OnDayChanged;

private:
  // �������, ������� ����� ���������� ������ �������
  void TickSecond();

  // --- ��������� �������� ��� ---
  float DayDurationInSeconds = 120.0f; // 2 ������

  // --- ��������� �������� ��� ---
  int32 CurrentDay = 1;
  float TimeElapsedInDay = 0.0f;
  int32 CurrentDayIncome = 0;
  int32 CurrentDayGoal = 100; // ���� �� ������ ����

  // ����� ��� ������ �������
  FTimerHandle SecondTickTimerHandle;
};