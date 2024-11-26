// copyright lolol

#include "Core/BTLGameMode.h"

#include "Core/BTLCharacter.h"

ABTLGameMode::ABTLGameMode()
{
	DefaultPawnClass = ABTLCharacter::StaticClass();
}
