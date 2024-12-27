// copyright lolol

#include "Core/BTLGameMode.h"

#include "Core/BTLCharacter.h"
#include "Core/BTLPlayerController.h"

ABTLGameMode::ABTLGameMode()
{
	DefaultPawnClass = ABTLCharacter::StaticClass();
	PlayerControllerClass = ABTLPlayerController::StaticClass();
}
