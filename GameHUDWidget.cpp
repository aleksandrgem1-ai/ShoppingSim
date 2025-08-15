// ShoppingSim/GameHUDWidget.cpp

#include "GameHUDWidget.h"
#include "ClockWidget.h"
#include "DayInfoWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MoneyHUDWidget.h"
#include "TimeManagerSubsystem.h"

void UGameHUDWidget::NativeConstruct() {
  Super::NativeConstruct();

  // The child widgets (ClockWidget, DayInfoWidget) will now update themselves
  // by binding to the TimeManagerSubsystem's delegates.
  // No need to manually call update functions from here.
}
