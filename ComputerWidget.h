// ComputerWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ComputerWidget.generated.h"

class UButton;
class UWidgetSwitcher;
class UFurnitureCatalogWidget;

UCLASS()
class SHOPPINGSIM_API UComputerWidget : public UUserWidget {
  GENERATED_BODY()

public:
  virtual bool Initialize() override;

    // Контейнер переключатель для приложений (рабочий стол)
  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
  UWidgetSwitcher *AppsSwitcher;

protected:
  // Кнопка для открытия магазина
  UPROPERTY(meta = (BindWidget))
  UButton *BtnOpenShop;

  // Ссылка на магазин (создаётся динамически)
  UPROPERTY()
  UFurnitureCatalogWidget *FurnitureCatalog;

  UFUNCTION()
  void OnOpenShopClicked();
};
