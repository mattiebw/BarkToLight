// copyright lolol

#include "WorldGen/LevelGeneratorBase.h"

#include "BarkToLightLog.h"

ALevelGeneratorBase::ALevelGeneratorBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALevelGeneratorBase::GenerateInEditor()
{
	Generate();
}

void ALevelGeneratorBase::Generate_Implementation()
{
	BTL_LOGC_ERROR(GetWorld(), "In ALevelGeneratorBase, Generate() called without implementation.");	
}
