// BuildModePawn.h
// Новый класс для камеры в режиме строительства

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "BuildModePawn.generated.h"

class UCameraComponent;
class UFloatingPawnMovement;
class UInputAction;

UCLASS()
class SHOPPINGSIM_API ABuildModePawn : public APawn {
  GENERATED_BODY()

public:
  ABuildModePawn();

protected:
  virtual void SetupPlayerInputComponent(
      class UInputComponent *PlayerInputComponent) override;

  // Компоненты
  UPROPERTY(VisibleAnywhere)
  TObjectPtr<UCameraComponent> CameraComponent;

  UPROPERTY(VisibleAnywhere)
  TObjectPtr<UFloatingPawnMovement> MovementComponent;

  // Ассеты ввода (теперь загружаются в .cpp)
  UPROPERTY()
  TObjectPtr<UInputAction> MoveAction;

  UPROPERTY()
  TObjectPtr<UInputAction> LookAction;

  UPROPERTY()
  TObjectPtr<UInputAction> FlyUpAction;

  UPROPERTY()
  TObjectPtr<UInputAction> FlyDownAction;

private:
  // Функции, привязанные к вводу
  void Move(const FInputActionValue &Value);
  void Look(const FInputActionValue &Value);
  void HandleFlyUp(const FInputActionValue &Value);
  void HandleFlyDown(const FInputActionValue &Value);
};