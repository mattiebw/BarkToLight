// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UTextBlock;
class AWeapon;
class ABTLPlayerCharacter;

/**
 * A C++ interface for the HUD widget.
 */
UCLASS()
class BARKTOLIGHT_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "HUD")
	void OnPawnPossessed(ABTLPlayerCharacter* Character);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "HUD")
	void OnWeaponSelected(AWeapon* Weapon);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD")
	void FadeOut(FLinearColor To = FLinearColor::Black, float Time = 0.5f);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD")
	void FadeIn(FLinearColor From, float Time = 0.5f);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD")
	void FadeOutAndIn(FLinearColor To         = FLinearColor::Black, float FadeOutTime = 0.5f, float HoldTime = 0.1f,
	                  float        FadeInTime = 0.5f);

	UFUNCTION(BlueprintNativeEvent, Category = "HUD")
	void OnHealthChanged(float NewHealth, float HealthDifference);

	UFUNCTION()
	void OnAmmoUpdated(int NewAmmo);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	ABTLPlayerCharacter* Player;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UWidget* AmmoContainer;
	
	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UTextBlock* MainAmmoText;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UTextBlock* ReserveAmmoText;

	UPROPERTY(BlueprintReadOnly)
	AWeapon* SelectedWeapon;
};
