// copyright lolol

#include "WorldGen/GenNavMeshBoundsDecorator.h"

#include "NavigationSystem.h"
#include "Builders/CubeBuilder.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "WorldGen/RoomsLevelGenerator.h"

void UGenNavMeshBoundsDecorator::Decorate()
{
	FBox Bounds;
	for (auto Room : Level->GetGeneratedRooms())
		Bounds += Room->GetComponentsBoundingBox();

	ANavMeshBoundsVolume *NavMeshBounds = Level->GetWorld()->SpawnActorDeferred<ANavMeshBoundsVolume>(ANavMeshBoundsVolume::StaticClass(), FTransform::Identity);
	UCubeBuilder* Cube = NewObject<UCubeBuilder>(NavMeshBounds, UCubeBuilder::StaticClass());
	Cube->X = Bounds.GetSize().X;
	Cube->Y = Bounds.GetSize().Y;
	Cube->Z = Bounds.GetSize().Z;
	NavMeshBounds->BrushBuilder = Cube;
	NavMeshBounds->FinishSpawning(FTransform(Bounds.GetCenter()));
	UNavigationSystemV1::GetNavigationSystem(this)->OnNavigationBoundsUpdated(NavMeshBounds);
	
	Level->GetOtherGeneratedActors().Add(NavMeshBounds);
	
	Super::Decorate();
}
