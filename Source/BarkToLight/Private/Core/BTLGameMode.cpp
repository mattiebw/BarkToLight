// copyright lolol

#include "Core/BTLGameMode.h"

#include "Core/BTLPlayerCharacter.h"
#include "Core/BTLPlayerController.h"

ABTLGameMode::ABTLGameMode()
{
	DefaultPawnClass = ABTLPlayerCharacter::StaticClass();
	PlayerControllerClass = ABTLPlayerController::StaticClass();
}
