// copyright lolol

#include "Layer/WorldGen/TreacheryConnector.h"

#include "Components/DynamicMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "WorldGen/BTLMeshGenerators.h"
#include "WorldGen/Room.h"
#include "WorldGen/RoomsLevelGenerator.h"

ATreacheryConnector::ATreacheryConnector()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh          = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void ATreacheryConnector::CreateSplineFromRooms_Implementation(ARoom* From, int FromIndex, ARoom* To, int ToIndex,
                                                               UBoundsChecker* BoundsChecker)
{
	Path->ClearSplinePoints();

	// First, lets create our basic spline points.
	FTransform FromConnectorTF = From->Connectors[FromIndex].Offset * From->GetTransform();
	FTransform ToConnectorTF   = To->Connectors[ToIndex].Offset * To->GetTransform();
	Path->AddSplinePoint(FromConnectorTF.GetLocation(), ESplineCoordinateSpace::World);
	Path->SetTangentAtSplinePoint(0, FromConnectorTF.GetRotation().GetForwardVector() * 2000,
	                              ESplineCoordinateSpace::World);
	Path->AddSplinePoint(ToConnectorTF.GetLocation(), ESplineCoordinateSpace::World);
	Path->SetTangentAtSplinePoint(1, -ToConnectorTF.GetRotation().GetForwardVector() * 2000,
	                              ESplineCoordinateSpace::World);

	// if (FromConnectorTF.Rotator().Vector().Dot(ToConnectorTF.Rotator().Vector()) > 0)
	// 	bIsValid = false;
	if (Path->GetSplineLength() > 25000)
		bIsValid = false;

	return; // Comment this line to enable the test code for spline avoidance.
	
	int Index = 1;
	for (float Time = 0.2f; Time < 0.8f; Time += 0.02f)
	{
		FVector Location = Path->GetLocationAtTime(Time, ESplineCoordinateSpace::World);

		FBox Box(Location - FVector(150), Location + FVector(150));
		if (!BoundsChecker->BoundsOverlapAnyBounds(Box))
			continue;

		FVector Dir   = Path->GetDirectionAtTime(Time, ESplineCoordinateSpace::World);
		FVector Right = UKismetMathLibrary::GetRightVector(Dir.ToOrientationRotator());
		FVector Orig = Location;

		for (int i = 0; i < 100; i++)
		{
			// Check for a free point to the right.
			Location = Orig + (Right * 150 * i);
			Box = FBox(Location - FVector(150), Location + FVector(150));
			if (!BoundsChecker->BoundsOverlapAnyBounds(Box))
				break;

			// Now to the left.
			Location = Orig + (-Right * 150 * i);
			Box = FBox(Location - FVector(150), Location + FVector(150));
			if (!BoundsChecker->BoundsOverlapAnyBounds(Box))
				break;
		}

		Path->AddSplinePointAtIndex(Location, Index, ESplineCoordinateSpace::World);
		Index++;
	}
}

void ATreacheryConnector::Generate_Implementation()
{
	UBTLMeshGenerators::GenerateTreacheryWalkway(Mesh, Path, FVector2D(350, 800), 0.002, 2000, 200, 0.06,
	                                             FMath::RandRange(0, 25000));
	PostGenerate();
}
