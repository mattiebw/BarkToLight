// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BTLMeshGenerators.generated.h"

class UDynamicMesh;
class UDynamicMeshComponent;

/**
 * Mesh generation functions for Bark To Light world generation.
 *
 * We put these here as functions instead of subclasses of ADynamicMeshActor or AGeneratedDynamicMeshActor as we 
 * want to be able to generate meshes as either. AGeneratedDynamicMeshActor is editor-only, as it bakes the mesh
 * into the level, which means it cannot be used in a packaged game. Therefore, we need to separate the mesh
 * generation code into a separate class if we want to take advantage of AGeneratedDynamicMeshActor's baking features
 * while also allowing the code to be reused for runtime generation.
 */
UCLASS()
class BARKTOLIGHT_API UBTLMeshGenerators : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Bark To Light|Mesh Generation")
	static UDynamicMesh* ComputeNormalsAndUVs(UDynamicMesh* Mesh, FVector2D UVScale = FVector2D(0.1f, 0.1f));

	UFUNCTION(BlueprintCallable, Category = "Bark To Light|Mesh Generation")
	static UDynamicMesh* ScaleMeshByVertexZ(UDynamicMesh* Mesh, float MinZ, float MaxZ, FVector MinScale, FVector MaxScale);

	UFUNCTION(BlueprintCallable, Category = "Bark To Light|Mesh Generation")
	static UDynamicMesh* PerlinNoiseDeformation(UDynamicMesh* Mesh, FGeometryScriptMeshSelection Selection,
	                                   FVector       PerAxisNoiseScale = FVector(1, 1, 1), int Seed = 0, float Magnitude = 5.0f, FVector Frequency = FVector(0.25));

	UFUNCTION(BlueprintCallable, Category = "Bark To Light|Mesh Generation")
	static void GenerateTreacheryFloor(UDynamicMeshComponent* MeshComponent, const FVector& Size, FVector PerAxisNoiseScale, FVector Frequency,
	                                   int32                  Seed);
};
