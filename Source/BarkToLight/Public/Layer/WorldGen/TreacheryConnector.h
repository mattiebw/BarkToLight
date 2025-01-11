// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "WorldGen/Connector.h"
#include "TreacheryConnector.generated.h"

class UDynamicMeshComponent;

UCLASS()
class BARKTOLIGHT_API ATreacheryConnector : public AConnector
{
	GENERATED_BODY()

public:
	ATreacheryConnector();

	virtual void CreateSplineFromRooms_Implementation(ARoom* From, int FromIndex, ARoom* To, int ToIndex, UBoundsChecker* BoundsChecker) override;

	virtual void Generate_Implementation() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Treachery Connector")
	UDynamicMeshComponent* Mesh;
};
