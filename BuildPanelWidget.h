// UI/HUD/BuildPanelWidget.h

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "BuildPanelWidget.generated.h"

class UButton;
class UFurniturePlaceManagerSubsystem;

/**
 * Панель управления строительством: кнопки для размещения, удаления, Undo/Redo
 */
UCLASS()
class SHOPPINGSIM_API UBuildPanelWidget : public UUserWidget {
  GENERATED_BODY()

protected:
  virtual void NativeConstruct() override;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UButton> PlaceButton;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UButton> RemoveButton;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UButton> UndoButton;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UButton> RedoButton;

private:
  UPROPERTY()
  TObjectPtr<UFurniturePlaceManagerSubsystem> PlaceManagerSubsystem;

  UFUNCTION()
  void OnPlaceClicked();

  UFUNCTION()
  void OnRemoveClicked();

  UFUNCTION()
  void OnUndoClicked();

  UFUNCTION()
  void OnRedoClicked();
};
