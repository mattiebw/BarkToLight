// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StatsClass.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatChanged, float, NewValue);

// These macros are used to declare stats in a class that inherits from UStatsClass.
// The DECLARE_BTL_STAT macro declares a stat with a default value, and creates a setter function and delegate.

#define DECLARE_BTL_STAT_CUSTOM_DELEGATE(Name, Type, Default, DelegateType) \
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = Set##Name, Category = "Stats") \
	Type Name = Default;\
	UPROPERTY(BlueprintAssignable, Category = "Stats") \
	DelegateType On##Name##Changed;\
	UFUNCTION(BlueprintCallable, Category = "Stats") \
	void Set##Name(Type NewValue) { Name = NewValue; On##Name##Changed.Broadcast(NewValue); }\

#define DECLARE_BTL_STAT(Name, Type, Default) DECLARE_BTL_STAT_CUSTOM_DELEGATE(Name, Type, Default, FOnStatChanged)

/**
 * Implements core behaviour for a class that holds stats (such as Weapon Stats, Enemy Stats, etc).
 */
UCLASS(EditInlineNew, BlueprintType, Blueprintable, HideCategories = (Object))
class BARKTOLIGHT_API UStatsClass : public UObject
{
	GENERATED_BODY()
};
