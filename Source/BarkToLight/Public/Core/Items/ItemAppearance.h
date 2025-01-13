// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemAppearance.generated.h"

/**
 * Represents an items appearance - name, icon, etc.
 */
UCLASS()
class BARKTOLIGHT_API UItemAppearance : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Appearance")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Appearance")
	UTexture2D* Texture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Appearance")
	UStaticMesh* Mesh;
};
