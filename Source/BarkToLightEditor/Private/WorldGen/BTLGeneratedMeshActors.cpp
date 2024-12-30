// copyright lolol

#include "BTLGeneratedMeshActors.h"

#include "WorldGen/BTLMeshGenerators.h"

void ATreacheryPlatformGeneratedMeshActor::RebuildGeneratedMesh(UDynamicMesh* TargetMesh)
{
	UBTLMeshGenerators::GenerateTreacheryFloor(GetDynamicMeshComponent(), Size, NoiseScale, Frequency, Seed);
}
