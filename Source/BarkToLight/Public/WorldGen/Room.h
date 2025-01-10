// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"

class ARoom;

UENUM(BlueprintType)
enum class ERoomConnectorType : uint8
{
	In,
	Out,
	Either
};

USTRUCT(BlueprintType)
struct FConnectorPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Connector", meta = (MakeEditWidget))
	FTransform Offset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Connector")
	ERoomConnectorType ConnectorType;

	UPROPERTY(BlueprintReadWrite, Category = "Connector")
	ARoom* ConnectedRoom;

	UPROPERTY(BlueprintReadWrite, Category = "Connector")
	int ConnectedToIndex = -1;

	UPROPERTY(BlueprintReadWrite, Category = "Connector")
	bool bConnected = false;
};

UCLASS()
class BARKTOLIGHT_API ARoom : public AActor
{
	GENERATED_BODY()

public:
	ARoom();

	virtual void Destroyed() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Room")
	bool HasAnyFreeConnectionPoints(bool bAllowIn = true, bool bAllowOut = true, bool bAllowBoth = true);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Room")
	FORCEINLINE bool IsPointFree(int PointIndex, bool bAllowIn = true, bool bAllowOut = true, bool bAllowBoth = true)
	{
		return Connectors.IsValidIndex(PointIndex)
		&& !Connectors[PointIndex].bConnected
		&& ((bAllowIn && Connectors[PointIndex].ConnectorType == ERoomConnectorType::In)
			|| (bAllowOut && Connectors[PointIndex].ConnectorType == ERoomConnectorType::Out)
			|| (bAllowBoth && Connectors[PointIndex].ConnectorType == ERoomConnectorType::Either));
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Room")
	bool RoomIsNotDeadEnd();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Room")
	int GetRandomFreeConnectionPointIndex(bool bAllowIn = true, bool bAllowOut = true, bool bAllowBoth = true);

	UFUNCTION(BlueprintCallable, Category = "Room")
	FORCEINLINE void MarkConnectionConnected(int Index, ERoomConnectorType NewConnectorType)
	{
		if (!Connectors.IsValidIndex(Index))
			return;

		Connectors[Index].bConnected = true;
		Connectors[Index].ConnectorType = NewConnectorType;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Room")
	int GetTotalAvailableOutwardConnections();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	TArray<FConnectorPoint> Connectors;

	UPROPERTY(BlueprintReadOnly, Category = "Room")
	TArray<AActor*> Parts;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	float Difficulty = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	bool bIsOnHotPath = false;
};
