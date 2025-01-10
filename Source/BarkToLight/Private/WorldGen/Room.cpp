// copyright lolol

#include "WorldGen/Room.h"

#include "BarkToLightLog.h"

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

int ARoom::GetTotalAvailableOutwardConnections()
{
	int Num = 0;
	for (const FConnectorPoint& Connector : Connectors)
	{
		if (!Connector.bConnected && (Connector.ConnectorType == ERoomConnectorType::Out || Connector.ConnectorType ==
			ERoomConnectorType::Either))
		{
			Num++;
		}
	}
	return Num;
}

void ARoom::ConnectTo(int ConnectorIndex, ARoom* OtherRoom, int OtherConnectorIndex)
{
	if (!Connectors.IsValidIndex(ConnectorIndex))
	{
		BTL_LOGC_ERROR(GetWorld(), "In ARoom::ConnectTo(), ConnectorIndex is out of range.");
		return;
	}

	if (!OtherRoom->Connectors.IsValidIndex(OtherConnectorIndex))
	{
		BTL_LOGC_ERROR(GetWorld(), "In ARoom::ConnectTo(), OtherConnectorIndex is out of range.");
		return;
	}

	Connectors[ConnectorIndex].bConnected = true;
	Connectors[ConnectorIndex].ConnectorType = ERoomConnectorType::Out;
	Connectors[ConnectorIndex].ConnectedRoom = OtherRoom;
	Connectors[ConnectorIndex].ConnectedToIndex = OtherConnectorIndex;
	
	OtherRoom->Connectors[OtherConnectorIndex].bConnected = true;
	OtherRoom->Connectors[OtherConnectorIndex].ConnectorType = ERoomConnectorType::In;
	OtherRoom->Connectors[OtherConnectorIndex].ConnectedRoom = this;
	OtherRoom->Connectors[OtherConnectorIndex].ConnectedToIndex = ConnectorIndex;
}
