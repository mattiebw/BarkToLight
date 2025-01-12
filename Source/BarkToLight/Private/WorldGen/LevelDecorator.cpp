// copyright lolol

#include "WorldGen/LevelDecorator.h"

void ULevelDecorator::Decorate()
{
	// The base class does nothing, and just defers to a possible blueprint implementation.
	OnDecorate();
}
