// EventManagerSubsystem.h (add Exec)
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EventManagerSubsystem.generated.h"

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

  // Консольная команда для ручной проверки (в PIE/Standalone)
  UFUNCTION(Exec)
  void EM_TestEvent();

private:
  void TryTriggerRandomEvent();

  // --- Типы событий ---
  void TriggerBreakdownEvent();
  void TriggerFineEvent();

  FTimerHandle EventTimerHandle;
};
