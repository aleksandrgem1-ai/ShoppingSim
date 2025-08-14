// SimCharacter.h (������ ������)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SimCharacter.generated.h"

class UCameraComponent;
class UInputAction;

UCLASS()
class SHOPPINGSIM_API ASimCharacter : public ACharacter {
  GENERATED_BODY()

public:
  ASimCharacter();

protected:
  virtual void BeginPlay() override;
  virtual void
  SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;
  virtual void Tick(float DeltaTime) override;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
  TObjectPtr<UCameraComponent> FollowCamera;

private:
  // --- Input Actions ��� ��������� ---
  UPROPERTY() TObjectPtr<UInputAction> MoveAction = nullptr;
  UPROPERTY() TObjectPtr<UInputAction> LookAction = nullptr;
  UPROPERTY() TObjectPtr<UInputAction> InteractAction = nullptr;

  // --- Input Action ��� ������������ ������ ---
  UPROPERTY() TObjectPtr<UInputAction> ToggleBuildModeAction;

  // --- �������-����������� ---
  void Move(const FInputActionValue &Value);
  void Look(const FInputActionValue &Value);
  void Interact(const FInputActionValue &Value);
  void ToggleBuildMode();

  void TraceForInteractable();

  UPROPERTY()
  TObjectPtr<class AInteractableActor> TargetedInteractable = nullptr;
};