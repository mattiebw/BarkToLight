// copyright lolol

#include "WorldGen/Connector.h"

#include "BarkToLightLog.h"
#include "Components/SplineComponent.h"

AConnector::AConnector()
{
	PrimaryActorTick.bCanEverTick = false;

	Path = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
}

void AConnector::Generate_Implementation()
{
	BTL_LOGC_ERROR_NOLOC(GetWorld(), "Generic AConnector Generate() called!");	
}
