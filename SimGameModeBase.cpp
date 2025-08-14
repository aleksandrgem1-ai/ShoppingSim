// SimGameModeBase.cpp
#include "SimGameModeBase.h"
#include "SimCharacter.h"
#include "SimPlayerController.h"

ASimGameModeBase::ASimGameModeBase() {
  DefaultPawnClass = ASimCharacter::StaticClass();
  PlayerControllerClass = ASimPlayerController::StaticClass();
}
