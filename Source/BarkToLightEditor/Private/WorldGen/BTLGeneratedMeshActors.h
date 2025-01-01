// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "GeometryActors/GeneratedDynamicMeshActor.h"
#include "BTLGeneratedMeshActors.generated.h"

class USplineComponent;

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

UCLASS()
class BARKTOLIGHTEDITOR_API ATreacheryWalkwayGeneratedMeshActor : public AGeneratedDynamicMeshActor
{
	GENERATED_BODY()

public:
	ATreacheryWalkwayGeneratedMeshActor();
	
	virtual void RebuildGeneratedMesh(UDynamicMesh* TargetMesh) override;

	UPROPERTY(EditAnywhere, Category = "Treachery Walkway")
	FVector2D Size = FVector2D(1, 1);

	UPROPERTY(EditAnywhere, Category = "Treachery Walkway")
	float Detail = 0.003f;

	UPROPERTY(EditAnywhere, Category = "Treachery Walkway")
	float SupportEvery = 500.f;
	
	UPROPERTY(EditAnywhere, Category = "Treachery Walkway")
	float NoiseScale;

	UPROPERTY(EditAnywhere, Category = "Treachery Walkway")
	float Frequency;
	
	UPROPERTY(EditAnywhere, Category = "Treachery Walkway")
	int32 Seed = 0;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Treachery Walkway")
	USplineComponent* Spline;
};
