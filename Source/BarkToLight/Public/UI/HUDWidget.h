// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "HUDWidget.generated.h"

class UWeaponChoiceUI;
class UVerticalBox;
class UInteractableComponent;
class UTextBlock;
class AWeapon;
class ABTLPlayerCharacter;

USTRUCT(BlueprintType)
struct FUINotification
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Notification")
	FText Content;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Notification")
	float Time = 3;
};

UCLASS()
class UNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetNotification(const FUINotification& Notification);

	virtual void NativeConstruct() override;
	
	FORCEINLINE void SetAlpha(float Alpha)
	{
		Text->SetColorAndOpacity(FSlateColor(FLinearColor(1, 1, 1, Alpha)));
	}

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "UI Notification")
	float Time = 1;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "UI Notification", meta = (BindWidget))
	UTextBlock* Text;
};

/**
 * A C++ implementation of the HUD widget.
 * This class is intended to be subclassed in Blueprints, which will implement the actual layout.
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

	UFUNCTION()
	void OnReserveAmmoUpdated();

	UFUNCTION()
	void OnInventoryAmmoUpdated(FName AmmoType, int NewAmmo);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void AddNotification(FText Content, float Time = 3);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateInteractable(UInteractableComponent* Interactable);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowPendingWeaponChoice(AWeapon* Weapon);
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	ABTLPlayerCharacter* Player;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UWidget* AmmoContainer;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UVerticalBox* PickupNotificationContainer;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UWidget* InteractablePromptContainer;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UTextBlock* InteractablePromptText;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UTextBlock* MainAmmoText;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UTextBlock* ReserveAmmoText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UNotificationWidget> NotificationWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UWeaponChoiceUI> WeaponChoiceWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	AWeapon* SelectedWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	TArray<UNotificationWidget*> NotificationWidgets;
};
