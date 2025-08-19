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
  TSubclassOf<class UGameHUDWidget> GameHUDWidgetClass;

  // --- Time Settings ---
  // How often the main time tick happens, in real-world seconds.
  UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Time")
  float TimeTickInterval = 1.0f;

  // The total duration of a single game day, in real-world seconds.
  UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Time",
            meta = (ClampMin = "60.0", UIMin = "60.0"))
  float DayDurationInSeconds = 1200.0f; // 20 minutes

  // --- Trace / Build Settings ---
  UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Trace")
  float TraceDistanceInteract = 15000.f;

  UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Trace")
  float TraceDistanceBuild = 15000.f;

  UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Trace")
  TEnumAsByte<ECollisionChannel> TraceChannelInteract = ECC_Visibility;

  UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Trace")
  TEnumAsByte<ECollisionChannel> TraceChannelBuild = ECC_Visibility;

  UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Build")
  float PlacementZOffset = 0.f;

  UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Build")
  bool bAlignToSurface = false;

  UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Build",
            meta = (ClampMin = "1.0", UIMin = "1.0"))
  float GridSize = 100.f;

  // --- Game Event Manager ---
  // Интервал появления случайных событий (сек)
  UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Events",
            meta = (ClampMin = "10.0", UIMin = "10.0"))
  float RandomEventInterval = 60.0f;

  // Вероятность события на тик (0.0–1.0)
  UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Events",
            meta = (ClampMin = "0.0", ClampMax = "1.0"))
  float RandomEventChance = 0.2f;
};
