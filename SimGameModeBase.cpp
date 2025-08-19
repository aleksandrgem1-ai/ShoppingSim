// SimGameModeBase.cpp 
#include "GameModes/SimGameModeBase.h"
#include "Characters/SimCharacter.h"
#include "Controllers/SimPlayerController.h"

ASimGameModeBase::ASimGameModeBase() {
  DefaultPawnClass = ASimCharacter::StaticClass();
  PlayerControllerClass = ASimPlayerController::StaticClass();
}
