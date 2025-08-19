// BuildModePawn.h (as provided, ok)
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
  virtual void BeginPlay() override;
  virtual void Tick(float DeltaTime) override;
  virtual void SetupPlayerInputComponent(
      class UInputComponent *PlayerInputComponent) override;

  UPROPERTY(VisibleAnywhere) TObjectPtr<UCameraComponent> CameraComponent;
  UPROPERTY(VisibleAnywhere)
  TObjectPtr<UFloatingPawnMovement> MovementComponent;

  // Input Actions
  UPROPERTY() TObjectPtr<UInputAction> MoveAction;
  UPROPERTY() TObjectPtr<UInputAction> LookAction;
  UPROPERTY() TObjectPtr<UInputAction> FlyUpAction;
  UPROPERTY() TObjectPtr<UInputAction> FlyDownAction;
  UPROPERTY() TObjectPtr<UInputAction> PlaceAction;
  UPROPERTY() TObjectPtr<UInputAction> RemoveAction;
  UPROPERTY() TObjectPtr<UInputAction> DestroyAction;
  UPROPERTY() TObjectPtr<UInputAction> RotateAction;
  UPROPERTY() TObjectPtr<UInputAction> ToggleGridAction;
  UPROPERTY() TObjectPtr<UInputAction> ExitBuildModeAction;
  UPROPERTY() TObjectPtr<UInputAction> ToggleBuildModeAction;

private:
  void Move(const FInputActionValue &Value);
  void Look(const FInputActionValue &Value);
  void HandleFlyUp(const FInputActionValue &Value);
  void HandleFlyDown(const FInputActionValue &Value);
  void HandlePlace();
  void HandleRemove();
  void HandleDestroy();
  void HandleRotate(const FInputActionValue &Value);
  void HandleToggleGrid();
  void HandleExitBuildMode();
};
