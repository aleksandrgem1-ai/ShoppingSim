// SimCharacter.h
#pragma once

#include "Components/InputComponent.h" // ����� ��� �������� ������
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
  virtual void Tick(float DeltaTime) override; // ��������� Tick

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
  UCameraComponent *FollowCamera;

private:
  UPROPERTY() UInputAction *MoveAction = nullptr;
  UPROPERTY() UInputAction *LookAction = nullptr;
  UPROPERTY() UInputAction *InteractAction = nullptr;

  void Move(const FInputActionValue &Value);
  void Look(const FInputActionValue &Value);
  void Interact(const FInputActionValue &Value);
  void TraceForInteractable();

  /** ��������� �� ������, �� ������� �� ������� � ������ ������ */
  UPROPERTY()
  TObjectPtr<class AInteractableActor> TargetedInteractable = nullptr;
};
