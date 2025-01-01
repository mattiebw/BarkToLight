// copyright lolol

#include "WorldGen/BTLMeshGenerators.h"

#include "Components/DynamicMeshComponent.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshUVFunctions.h"
#include "GeometryScript/MeshNormalsFunctions.h"
#include "GeometryScript/MeshSelectionFunctions.h"
#include "GeometryScript/MeshSubdivideFunctions.h"
#include "GeometryScript/MeshDeformFunctions.h"
#include "GeometryScript/SceneUtilityFunctions.h"
#include "GeometryScript/PolyPathFunctions.h"
#include "GeometryScript/MeshMaterialFunctions.h"
#include "GeometryScript/MeshPolygroupFunctions.h"
#include "GeometryScript/MeshBasicEditFunctions.h"
#include "GeometryScript/CollisionFunctions.h"
#include "Kismet/KismetMathLibrary.h"

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
                                                     FVector       MaxScale)
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

void UBTLMeshGenerators::GenerateTreacheryWalkway(UDynamicMeshComponent* MeshComponent, USplineComponent* Spline,
                                                  FVector2D              Size, float Detail, float SupportEvery,
                                                  float                  NoiseScale, float Frequency, int32 Seed)
{
	// Setup the mesh
	UDynamicMesh* Mesh = MeshComponent->GetDynamicMesh();
	UGeometryScriptLibrary_MeshPolygroupFunctions::EnablePolygroups(Mesh);
	UGeometryScriptLibrary_MeshMaterialFunctions::EnableMaterialIDs(Mesh);

	// Sample the spline to transforms so we can extrude the rectangle along it:
	TArray<FTransform>                   Transforms;
	TArray<double>                       FrameTimes; // Unused
	FGeometryScriptSplineSamplingOptions Options;
	Options.NumSamples = Spline->GetSplineLength() * Detail;
	UGeometryScriptLibrary_PolyPathFunctions::SampleSplineToTransforms(Spline, Transforms, FrameTimes, Options,
	                                                                   Spline->GetRelativeTransform());

	// Make our basic rectangle:
	TArray<FVector2D> Polygon;
	Polygon.Add(FVector2D(-0.5, 0) * Size); // 0 for the Y so it's flat on the ground
	Polygon.Add(FVector2D(0.5, 0) * Size);
	Polygon.Add(FVector2D(1.5, 1) * Size);
	Polygon.Add(FVector2D(-1.5, 1) * Size); // Larger X for the higher Y so we give it some depth for visual interest.

	// Extrude it along the spline:
	FGeometryScriptPrimitiveOptions PrimitiveOptions;
	UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendSweepPolygon(Mesh, PrimitiveOptions, FTransform(), Polygon,
	                                                                  Transforms, false, true, 1, 1);

	// Apply tesselation and noise:
	UGeometryScriptLibrary_MeshSubdivideFunctions::ApplyUniformTessellation(Mesh);
	FGeometryScriptMeshSelection Selection;
	UGeometryScriptLibrary_MeshSelectionFunctions::CreateSelectAllMeshSelection(Mesh, Selection);
	FGeometryScriptPerlinNoiseOptions PerlinNoiseOptions;
	PerlinNoiseOptions.bApplyAlongNormal    = true;
	PerlinNoiseOptions.BaseLayer.RandomSeed = Seed;
	PerlinNoiseOptions.BaseLayer.Magnitude  = NoiseScale;
	PerlinNoiseOptions.BaseLayer.Frequency  = Frequency;
	UGeometryScriptLibrary_MeshDeformFunctions::ApplyPerlinNoiseToMesh(Mesh, Selection, PerlinNoiseOptions);

	// Setup the material for that section:
	UGeometryScriptLibrary_MeshSelectionFunctions::CreateSelectAllMeshSelection(Mesh, Selection);
	UGeometryScriptLibrary_MeshMaterialFunctions::SetMaterialIDForMeshSelection(Mesh, Selection, 0);

	// Now, let's add the supports. We'll need to make a dynamic mesh pool so we can have a separate mesh for the supports,
	// so that we can apply a different material to them.
	UDynamicMeshPool* Pool = UGeometryScriptLibrary_SceneUtilityFunctions::CreateDynamicMeshPool();
	UDynamicMesh* SupportsMesh = Pool->RequestMesh(); // This should all be garbage collected when we're done with it.

	// Generate the supports.
	FGeometryScriptPrimitiveOptions SupportOptions;
	float                           Time         = SupportEvery;
	bool                            Side         = false; // Left is false, right is true.
	float                           SplineLength = Spline->GetSplineLength();
	while (Time < SplineLength)
	{
		FVector Position  = Spline->GetLocationAtDistanceAlongSpline(Time, ESplineCoordinateSpace::Local);
		FVector Direction = Spline->GetDirectionAtDistanceAlongSpline(Time, ESplineCoordinateSpace::Local);
		Position += UKismetMathLibrary::GetRightVector(Direction.ToOrientationRotator()) * ((Size.X * 0.8f) * (
			Side ? 1 : -1));

		UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox(SupportsMesh, SupportOptions,
		                                                         FTransform(Position + FVector(0, 0, Size.Y / 2)), 100,
		                                                         100, 60000);

		Side = !Side;
		Time += SupportEvery;
	}

	// Apply our supports material to the supports.
	UGeometryScriptLibrary_MeshSelectionFunctions::CreateSelectAllMeshSelection(SupportsMesh, Selection);
	UGeometryScriptLibrary_MeshMaterialFunctions::SetMaterialIDForMeshSelection(SupportsMesh, Selection, 1);

	// Append our supports mesh to our main mesh.
	UGeometryScriptLibrary_MeshBasicEditFunctions::AppendMesh(Mesh, SupportsMesh, FTransform());

	// Now finish up by calculating normals.
	FGeometryScriptSplitNormalsOptions SplitNormalsOptions;
	SplitNormalsOptions.bSplitByOpeningAngle = true;
	SplitNormalsOptions.OpeningAngleDeg      = 0.5f;
	FGeometryScriptCalculateNormalsOptions CalculateNormalsOptions;
	UGeometryScriptLibrary_MeshNormalsFunctions::ComputeSplitNormals(Mesh, SplitNormalsOptions,
	                                                                 CalculateNormalsOptions);
	// ComputeNormalsAndUVs(Mesh); // Disabled for now - seems worse than without it!
	
	// Set our collision settings.
	// FGeometryScriptCollisionFromMeshOptions CollisionOptions;
	UGeometryScriptLibrary_CollisionFunctions::ResetDynamicMeshCollision(MeshComponent);
	// UGeometryScriptLibrary_CollisionFunctions::SetDynamicMeshCollisionFromMesh(Mesh, MeshComponent, CollisionOptions);
	MeshComponent->EnableComplexAsSimpleCollision();
}
