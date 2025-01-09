// copyright lolol

#include "WorldGen/Room.h"

ARoom::ARoom()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARoom::Destroyed()
{
	Super::Destroyed();

	// We should also destroy all of the rooms parts when the room itself is destroyed.
	for (auto Part : Parts)
		Part->Destroy();
}

bool ARoom::HasAnyFreeConnectionPoints(bool bAllowIn, bool bAllowOut, bool bAllowBoth)
{
	for (int i = 0; i < Connectors.Num(); i++)
	{
		if (IsPointFree(i, bAllowIn, bAllowOut, bAllowBoth))
			return true;
	}

	return false;
}

bool ARoom::RoomIsNotDeadEnd()
{
	bool bSeenOtherEitherConnector = false;
	for (const FConnectorPoint& Connector : Connectors)
	{
		if (Connector.bConnected)
			continue;

		if (Connector.ConnectorType == ERoomConnectorType::Out)
			return true;

		if (Connector.ConnectorType == ERoomConnectorType::Either)
		{
			if (bSeenOtherEitherConnector)
				return true;
			bSeenOtherEitherConnector = true;
		}
	}

	return false;
}

int ARoom::GetRandomFreeConnectionPointIndex(bool bAllowIn, bool bAllowOut, bool bAllowBoth)
{
	int FreePoints = 0;
	for (int i = 0; i < Connectors.Num(); i++)
		if (IsPointFree(i, bAllowIn, bAllowOut, bAllowBoth))
			FreePoints++;
	if (FreePoints == 0)
		return -1;
	
	int PointIndex = FMath::RandRange(0, FreePoints - 1);

	for (int i = 0; i < Connectors.Num(); i++)
	{
		if (IsPointFree(i, bAllowIn, bAllowOut, bAllowBoth))
		{
			if (PointIndex == 0)
				return i;
			PointIndex--;
		}
	}

	return -1;
}
