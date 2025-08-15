// ShoppingSim/SimSettings.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SimSettings.generated.h"

/**
 * Game-wide settings for the simulation.
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Sim Game Settings"))
class SHOPPINGSIM_API USimSettings : public UDeveloperSettings {
  GENERATED_BODY()

public:
  // --- UI Settings ---
  UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "UI",
            meta = (AllowedClasses = "/Script/ShoppingSim.GameHUDWidget"))
  TSubclassOf<class UUserWidget> GameHUDWidgetClass;

  // --- Time Settings ---
  // How often the main time tick happens, in real-world seconds.
  UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Time")
  float TimeTickInterval = 1.0f;

  // The total duration of a single game day, in real-world seconds.
  UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Time",
            meta = (ClampMin = "60.0", UIMin = "60.0"))
  float DayDurationInSeconds = 1200.0f; // Default to 20 minutes
};
