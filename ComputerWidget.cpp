// ComputerWidget.cpp
#include "UI/HUD/ComputerWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/HUD/FurnitureCatalogWidget.h"

bool UComputerWidget::Initialize() {
  bool bSuccess = Super::Initialize();
  if (!bSuccess)
    return false;

  if (BtnOpenShop) {
    BtnOpenShop->OnClicked.AddDynamic(this,
                                      &UComputerWidget::OnOpenShopClicked);
  }

  return true;
}

void UComputerWidget::OnOpenShopClicked() {
  if (!FurnitureCatalog && AppsSwitcher) {
    FurnitureCatalog = CreateWidget<UFurnitureCatalogWidget>(
        GetWorld(), UFurnitureCatalogWidget::StaticClass());
    if (FurnitureCatalog) {
      AppsSwitcher->AddChild(FurnitureCatalog);
    }
  }

  if (AppsSwitcher) {
    // Предполагаем, что магазин добавлен последним, переключаемся на него
    AppsSwitcher->SetActiveWidget(FurnitureCatalog);
  }
}
