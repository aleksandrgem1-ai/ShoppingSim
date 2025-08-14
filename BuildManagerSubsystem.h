// BuildManagerSubsystem.h(ОБНОВЛЕННЫЙ)

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BuildManagerSubsystem.generated.h"

class UStoreZoneData;
class AStoreZoneActor;
class UInputMappingContext;

UCLASS()
class SHOPPINGSIM_API UBuildManagerSubsystem : public UGameInstanceSubsystem {
  GENERATED_BODY()

public:
  // Стандартная функция инициализации для подсистем
  virtual void Initialize(FSubsystemCollectionBase &Collection) override;

  // Вход в режим строительства
  UFUNCTION(BlueprintCallable, Category = "Building")
  void EnterBuildMode();

  // Выход из режима строительства
  UFUNCTION(BlueprintCallable, Category = "Building")
  void ExitBuildMode();

  // ... остальной код ...

private:
  // Сохраняем указатель на оригинального персонажа
  UPROPERTY()
  TObjectPtr<APawn> OriginalPlayerPawn;

  // Ассеты IMC (теперь загружаются в .cpp)
  UPROPERTY()
  TObjectPtr<UInputMappingContext> PlayerMappingContext;

  UPROPERTY()
  TObjectPtr<UInputMappingContext> BuildModeMappingContext;

  // Класс для камеры (теперь устанавливается в .cpp)
  TSubclassOf<APawn> BuildModePawnClass;
};