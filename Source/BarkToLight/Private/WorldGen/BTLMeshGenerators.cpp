// copyright lolol

#include "WorldGen/BTLMeshGenerators.h"

#include "Components/DynamicMeshComponent.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshUVFunctions.h"
#include "GeometryScript/MeshNormalsFunctions.h"
#include "GeometryScript/MeshSelectionFunctions.h"
#include "GeometryScript/MeshSubdivideFunctions.h"
#include "GeometryScript/MeshDeformFunctions.h"

UDynamicMesh* UBTLMeshGenerators::ComputeNormalsAndUVs(UDynamicMesh* Mesh, FVector2D UVScale)
{
	FGeometryScriptCalculateNormalsOptions CalculateNormalsOptions;
	UGeometryScriptLibrary_MeshNormalsFunctions::RecomputeNormals(Mesh, CalculateNormalsOptions);

	FGeometryScriptRecomputeUVsOptions RecomputeUVsOptions;
	FGeometryScriptMeshSelection       Selection;
	UGeometryScriptLibrary_MeshSelectionFunctions::CreateSelectAllMeshSelection(Mesh, Selection);
	UGeometryScriptLibrary_MeshUVFunctions::RecomputeMeshUVs(Mesh, 0, RecomputeUVsOptions, Selection);

	UGeometryScriptLibrary_MeshUVFunctions::ScaleMeshUVs(Mesh, 0, UVScale, FVector2D(0, 0), Selection);

	return Mesh;
}

UDynamicMesh* UBTLMeshGenerators::ScaleMeshByVertexZ(UDynamicMesh* Mesh, float MinZ, float MaxZ, FVector MinScale,
	FVector MaxScale)
{
	return Mesh;
}

UDynamicMesh* UBTLMeshGenerators::PerlinNoiseDeformation(UDynamicMesh* Mesh, FGeometryScriptMeshSelection Selection,
                                                         FVector       PerAxisNoiseScale, int Seed, float Magnitude,
                                                         FVector       Frequency)
{
	Mesh->EditMesh([&](FDynamicMesh3& EditMesh)
	{
		FRandomStream Random(Seed);
		FVector3d     Offsets[3];
		for (int k = 0; k < 3; ++k)
		{
			const float RandomOffset = 10000.0f * Random.GetFraction();
			Offsets[k]               = FVector3d(RandomOffset, RandomOffset, RandomOffset);
			// Offsets[k] += (FVector3d)Options.BaseLayer.FrequencyShift;
		}

		auto GetDisplacedPosition = [&EditMesh, &PerAxisNoiseScale, &Offsets, Magnitude, Frequency](int32 VertexID)
		{
			FVector3d Pos = EditMesh.GetVertex(VertexID);
			// if (bAlignWithNormal)
			// {
			// 	FVector NoisePos = (FVector)((double)Options.BaseLayer.Frequency * (Pos + Offsets[0]));
			// 	float Displacement = Options.BaseLayer.Magnitude * FMath::PerlinNoise3D(Options.BaseLayer.Frequency * NoisePos);
			// 	Pos += Displacement * Normals[VertexID];
			// }
			// else
			// {
			FVector3d Displacement;
			for (int32 k = 0; k < 3; ++k)
			{
				// FVector NoisePos = static_cast<double>(Frequency) * (Pos + Offsets[k]);
				FVector NoisePos = Frequency * (Pos + Offsets[k]);
				Displacement[k]  = Magnitude * FMath::PerlinNoise3D(NoisePos);
			}
			Pos += Displacement * PerAxisNoiseScale;
			// }
			return Pos;
		};

		if (Selection.IsEmpty())
		{
			ParallelFor(EditMesh.MaxVertexID(), [&](int32 VertexID)
			{
				if (EditMesh.IsVertex(VertexID))
				{
					EditMesh.SetVertex(VertexID, GetDisplacedPosition(VertexID));
				}
			});
		}
		else
		{
			Selection.ProcessByVertexID(EditMesh, [&](int32 VertexID)
			{
				EditMesh.SetVertex(VertexID, GetDisplacedPosition(VertexID));
			});
		}
	}, EDynamicMeshChangeType::GeneralEdit, EDynamicMeshAttributeChangeFlags::Unknown, false);

	return Mesh;
}

void UBTLMeshGenerators::GenerateTreacheryFloor(UDynamicMeshComponent* MeshComponent, const FVector& Size,
                                                FVector                PerAxisNoiseScale, FVector    Frequency,
                                                int32                  Seed)
{
	UDynamicMesh* Mesh = MeshComponent->GetDynamicMesh();

	FGeometryScriptPrimitiveOptions BoxOptions;
	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox(Mesh, BoxOptions, FTransform(), Size.X, Size.Y, Size.Z);

	UGeometryScriptLibrary_MeshSubdivideFunctions::ApplyUniformTessellation(Mesh, 10);

	

	ComputeNormalsAndUVs(Mesh);
}
