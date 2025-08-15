// EventManagerSubsystem.h(����� ����)

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EventManagerSubsystem.generated.h"

    // ������� ��� ���������� UI � �������
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEventSignature,
                                                const FText &, EventText);

UCLASS()
class SHOPPINGSIM_API UEventManagerSubsystem : public UWorldSubsystem {
  GENERATED_BODY()

public:
  virtual void Initialize(FSubsystemCollectionBase &Collection) override;
  virtual void Deinitialize() override;

  UPROPERTY(BlueprintAssignable, Category = "Events")
  FOnGameEventSignature OnGameEvent;

private:
  void TryTriggerRandomEvent();

  // --- ���� ������� ---
  void TriggerBreakdownEvent();
  void TriggerFineEvent();

  FTimerHandle EventTimerHandle;
};