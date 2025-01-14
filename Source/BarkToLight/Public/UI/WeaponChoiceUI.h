// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponChoiceUI.generated.h"

class UHorizontalBox;
class UTextBlock;
class AWeapon;
class ABTLPlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChoice, AWeapon*, Weapon);

// Base C++ class for the UI used to represent a weapon.
// Intended to be subclassed in Blueprints, where the actual UI design will be implemented.
UCLASS()
class UWeaponUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Weapon UI")
	void SetWeapon(AWeapon* InWeapon);

	UFUNCTION(BlueprintCallable, Category = "Weapon UI")
	void ChoiceMade();
	
	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "Weapon UI")
	FOnWeaponChoice OnWeaponChoice;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon UI")
	AWeapon* Weapon;
	
	UPROPERTY(BlueprintReadOnly, Category = "Weapon UI", meta = (BindWidget))
	UTextBlock* WeaponName;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon UI", meta = (BindWidget))
	UTextBlock* WeaponUpgrades;
};

/**
 * Base C++ class for the UI shown when weapon slots are full and a choice to discard a weapon must be made.
 * Intended to be subclassed in Blueprints, where the actual UI design will be implemented.
 */
UCLASS()
class BARKTOLIGHT_API UWeaponChoiceUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Weapon Choice")
	void Show(ABTLPlayerCharacter* InPlayer, AWeapon* InNewWeapon);

protected:
	UFUNCTION()
	void OnWeaponSelected(AWeapon* Weapon);
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon Choice")
	AWeapon* NewWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Choice")
	TSubclassOf<UWeaponUI> WeaponUIWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Choice", meta = (BindWidget))
	UHorizontalBox* CurrentWeaponList;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon Choice")
	ABTLPlayerCharacter* Player;
	
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Choice", meta = (BindWidget))
	UWeaponUI* NewWeaponUI = nullptr;
};
