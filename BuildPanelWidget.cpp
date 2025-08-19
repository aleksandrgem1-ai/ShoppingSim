// UI/HUD/Private/BuildPanelWidget.cpp

#include "UI/HUD/BuildPanelWidget.h"
#include "Components/Button.h"
#include "Subsystems/FurniturePlaceManagerSubsystem.h"

void UBuildPanelWidget::NativeConstruct() {
  Super::NativeConstruct();

  PlaceManagerSubsystem =
      GetWorld() ? GetWorld()
                       ->GetGameInstance()
                       ->GetSubsystem<UFurniturePlaceManagerSubsystem>()
                 : nullptr;

  if (PlaceButton)
    PlaceButton->OnClicked.AddDynamic(this, &UBuildPanelWidget::OnPlaceClicked);

  if (RemoveButton)
    RemoveButton->OnClicked.AddDynamic(this,
                                       &UBuildPanelWidget::OnRemoveClicked);

  if (UndoButton)
    UndoButton->OnClicked.AddDynamic(this, &UBuildPanelWidget::OnUndoClicked);

  if (RedoButton)
    RedoButton->OnClicked.AddDynamic(this, &UBuildPanelWidget::OnRedoClicked);
}

void UBuildPanelWidget::OnPlaceClicked() {
  if (PlaceManagerSubsystem) {
    // Логика включения режима размещения (например, UI должен передать
    // Furniture) Здесь пример вызова без параметров
    PlaceManagerSubsystem->ExitPlacementMode(); // выход
  }
}

void UBuildPanelWidget::OnRemoveClicked() {
  // Вызов соответствующей логики удаления мебели из PlaceManagerSubsystem
}

void UBuildPanelWidget::OnUndoClicked() {
  if (PlaceManagerSubsystem) {
    PlaceManagerSubsystem->UndoLastAction();
  }
}

void UBuildPanelWidget::OnRedoClicked() {
  if (PlaceManagerSubsystem) {
    PlaceManagerSubsystem->RedoLastAction();
  }
}
