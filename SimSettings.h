#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Blueprint/UserWidget.h" // Нужно для TSubclassOf<UUserWidget>
#include "SimSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Sim Game Settings"))
class SHOPPINGSIM_API USimSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    USimSettings() {}

    /** Класс виджета для отображения денег игрока */
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> MoneyHUDWidgetClass;
};