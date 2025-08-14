// SimPlayerController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SimPlayerController.generated.h"

class UInputMappingContext;
class UMoneyHUDWidget; // forward declaration

UCLASS()
class SHOPPINGSIM_API ASimPlayerController : public APlayerController {
  GENERATED_BODY()

public:
  ASimPlayerController();

  UFUNCTION(BlueprintCallable, Exec, Category = "Money")
  void AddMoney(int32 Amount);

  int32 LoadCurrentMoneyFromGameStateOrSave() const;

protected:
  virtual void BeginPlay() override;
  virtual void OnPossess(APawn *InPawn) override;

private:
  // IMC загружается из контента в конструкторе
  UPROPERTY()
  UInputMappingContext *DefaultMappingContext = nullptr;

  UPROPERTY()
  int32 MappingPriority = 0;

  void AddDefaultIMC();

  // Это свойство теперь будет заполнено из настроек проекта в .cpp файле
  TSubclassOf<UMoneyHUDWidget> MoneyHUDClass;

  UPROPERTY(Transient)
  UMoneyHUDWidget *MoneyHUD = nullptr;

  UPROPERTY(VisibleInstanceOnly, Category = "Money")
  int32 CurrentMoney = 0;
};