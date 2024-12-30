// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageSource.generated.h"

UENUM(BlueprintType)
enum class EDamageSourceType : uint8
{
	Player,
	Enemy,
	Environment,
};

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UDamageSource : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface should be implemented by any actor that can deal damage to another actor, and should then appear
 * in the damage report, and expect to receive effects from the damage (such as thorns).
 */
class BARKTOLIGHT_API IDamageSource
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Source")
	FText GetDamageSourceName() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Source")
	EDamageSourceType GetDamageSourceType() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Source")
	AActor* GetDamageSource();
};
