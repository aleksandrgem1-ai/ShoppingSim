// SimPlayerController.h (»—ѕ–ј¬Ћ≈ЌЌјя ¬≈–—»я)
// ћы убираем все упоминани€ CurrentMoney, AddMoney, Load... и OnPossess,
// так как этой логики в контроллере больше нет.

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

protected:
  // BeginPlay - это единственна€ функци€ жизненного цикла, котора€ нам здесь
  // нужна
  virtual void BeginPlay() override;

private:
  // IMC загружаетс€ из контента в конструкторе
  UPROPERTY()
  TObjectPtr<UInputMappingContext> DefaultMappingContext = nullptr;

  UPROPERTY()
  int32 MappingPriority = 0;

  void AddDefaultIMC();

  // Ёто свойство теперь будет заполнено из настроек проекта в .cpp файле
  TSubclassOf<UMoneyHUDWidget> MoneyHUDClass;

  // ”казатель на созданный виджет дл€ HUD
  UPROPERTY(Transient)
  TObjectPtr<UMoneyHUDWidget> MoneyHUD = nullptr;
};