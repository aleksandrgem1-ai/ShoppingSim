// EventManagerSubsystem.h(НОВЫЙ ФАЙЛ)

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EventManagerSubsystem.generated.h"

    // Делегат для оповещения UI о событии
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

  // --- Типы событий ---
  void TriggerBreakdownEvent();
  void TriggerFineEvent();

  FTimerHandle EventTimerHandle;
};