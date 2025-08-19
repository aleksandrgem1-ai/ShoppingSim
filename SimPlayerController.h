#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SimPlayerController.generated.h"

class UInputMappingContext;
class UGameHUDWidget;
class UEventManagerSubsystem;

UCLASS()
class SHOPPINGSIM_API ASimPlayerController : public APlayerController {
  GENERATED_BODY()

public:
  ASimPlayerController();

protected:
  virtual void BeginPlay() override;

private:
  void AddDefaultIMC();

  UPROPERTY()
  TObjectPtr<UInputMappingContext> DefaultMappingContext = nullptr;

  int32 MappingPriority = 0;

  UPROPERTY(EditDefaultsOnly, Category = "UI")
  TSubclassOf<UGameHUDWidget> GameHUDClass;

  UPROPERTY(Transient)
  TObjectPtr<UGameHUDWidget> GameHUD = nullptr;

  UFUNCTION()
  void HandleGameEvent(const FText &EventText);
};
