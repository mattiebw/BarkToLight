// copyright lolol

#include "BTLGeneratedMeshActors.h"

#include "Components/SplineComponent.h"
#include "WorldGen/BTLMeshGenerators.h"

void ATreacheryPlatformGeneratedMeshActor::RebuildGeneratedMesh(UDynamicMesh* TargetMesh)
{
	UBTLMeshGenerators::GenerateTreacheryFloor(GetDynamicMeshComponent(), Size, NoiseScale, Frequency, Seed);
}

ATreacheryWalkwayGeneratedMeshActor::ATreacheryWalkwayGeneratedMeshActor()
{
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);
}

void ATreacheryWalkwayGeneratedMeshActor::RebuildGeneratedMesh(UDynamicMesh* TargetMesh)
{
	UBTLMeshGenerators::GenerateTreacheryWalkway(GetDynamicMeshComponent(), Spline, Size, Detail, SupportEvery, NoiseScale, Frequency, Seed);
}
