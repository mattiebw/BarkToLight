// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Item.generated.h"

class UItemAppearance;

/**
 * The base class for items. Very generalised.
 */
UCLASS()
class BARKTOLIGHT_API UItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	UItemAppearance* Appearance;
};
