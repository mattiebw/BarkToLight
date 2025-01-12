// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StatsClass.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatChanged, float, NewValue);

// These macros are used to declare stats in a class that inherits from UStatsClass.
// The DECLARE_BTL_STAT macro declares a stat with a default value, and creates a setter function and delegate.

#define DECLARE_BTL_STAT_CUSTOM_DELEGATE(Name, Type, Default, DelegateType) \
	protected: \
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = Set##Name, Category = "Stats") \
	Type Name = Default;\
	public: \
	UPROPERTY(BlueprintAssignable, Category = "Stats") \
	DelegateType On##Name##Changed;\
	UFUNCTION(BlueprintCallable, Category = "Stats") \
	void Set##Name(Type NewValue) { Name = NewValue; On##Name##Changed.Broadcast(GetValue(#Name)); }\
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats") \
	Type Get##Name() const { return GetValue(#Name); }\

#define DECLARE_BTL_STAT(Name, Type, Default) DECLARE_BTL_STAT_CUSTOM_DELEGATE(Name, Type, Default, FOnStatChanged)

UENUM(BlueprintType)
enum class EUpgradeType : uint8
{
	Add,
	Multiply,
};

USTRUCT(BlueprintType)
struct FStatUpgrade
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Upgrade")
	FName StatName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Upgrade")
	EUpgradeType UpgradeType = EUpgradeType::Add;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat Upgrade")
	float Value = 1;
};

/**
 * Implements core behaviour for a class that holds stats (such as Weapon Stats, Enemy Stats, etc).
 */
UCLASS(EditInlineNew, BlueprintType, Blueprintable, HideCategories = (Object))
class BARKTOLIGHT_API UStatsClass : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddUpgrade(const FStatUpgrade& Upgrade);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetValue(FName StatName) const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	TArray<FStatUpgrade> StatUpgrades;
};
