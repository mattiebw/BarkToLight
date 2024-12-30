// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "GeometryActors/GeneratedDynamicMeshActor.h"
#include "BTLGeneratedMeshActors.generated.h"

UCLASS()
class BARKTOLIGHTEDITOR_API ATreacheryPlatformGeneratedMeshActor : public AGeneratedDynamicMeshActor
{
	GENERATED_BODY()

public:
	virtual void RebuildGeneratedMesh(UDynamicMesh* TargetMesh) override;

	UPROPERTY(EditAnywhere)
	FVector Size = FVector(1000, 400, 500);
	
	UPROPERTY(EditAnywhere)
	FVector NoiseScale;

	UPROPERTY(EditAnywhere)
	FVector Frequency;
	
	UPROPERTY(EditAnywhere)
	int32 Seed = 0;
};
