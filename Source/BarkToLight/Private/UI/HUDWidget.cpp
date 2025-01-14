// copyright lolol

#include "UI/HUDWidget.h"

#include "BarkToLightLog.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Core/BTLPlayerCharacter.h"
#include "Core/BTLPlayerController.h"
#include "Core/HealthComponent.h"
#include "Core/InteractableComponent.h"
#include "UI/WeaponChoiceUI.h"
#include "Weapon/Weapon.h"

void UHUDWidget::OnPawnPossessed_Implementation(ABTLPlayerCharacter* Character)
{
	// First, remove any existing listeners.
	if (Player)
	{
		Player->GetHealthComponent()->OnHealthChanged.RemoveAll(this);
	}

	Player = Character;

	// Bind to the new player's health component.
	Player->GetHealthComponent()->OnHealthChanged.AddDynamic(this, &UHUDWidget::OnHealthChanged);
	Player->GetInventoryComponent()->OnAmmoUpdated.AddDynamic(this, &UHUDWidget::OnInventoryAmmoUpdated);

	// Update the health text immediately.
	OnHealthChanged(Player->GetHealthComponent()->GetHealth(), 0);
}

void UHUDWidget::OnWeaponSelected_Implementation(AWeapon* Weapon)
{
	if (SelectedWeapon != nullptr)
	{
		Weapon->OnAmmoUpdated.RemoveAll(this);
	}

	SelectedWeapon = Weapon;

	if (SelectedWeapon == nullptr)
	{
		AmmoContainer->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	AmmoContainer->SetVisibility(ESlateVisibility::Visible);

	Weapon->OnAmmoUpdated.AddUniqueDynamic(this, &UHUDWidget::OnAmmoUpdated);

	switch (SelectedWeapon->GetData()->AmmoUsageType)
	{
	case EAmmoUsageType::InfiniteAmmo:
		ReserveAmmoText->SetVisibility(ESlateVisibility::Collapsed);
		MainAmmoText->SetText(FText::FromString(TEXT("∞")));
		break;
	case EAmmoUsageType::Charge:
		MainAmmoText->SetVisibility(ESlateVisibility::Collapsed);
		ReserveAmmoText->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EAmmoUsageType::ClipAndReserve:
	case EAmmoUsageType::SinglePool:
		OnAmmoUpdated(SelectedWeapon->GetLoadedAmmo());
		break;
	}
}

void UHUDWidget::OnHealthChanged_Implementation(float NewHealth, float HealthDifference)
{
	HealthText->SetText(FText::Format(FText::FromString("{0}"), (int)NewHealth));
}

void UNotificationWidget::SetNotification(const FUINotification& Notification)
{
	Text->SetText(Notification.Content);
	Time = Notification.Time;
}

void UNotificationWidget::NativeConstruct()
{
	Text->SetColorAndOpacity(FSlateColor(FLinearColor(1, 1, 1, 1)));
}

void UHUDWidget::OnAmmoUpdated(int NewAmmo)
{
	MainAmmoText->SetVisibility(ESlateVisibility::Visible);
	MainAmmoText->SetText(FText::FromString(FString::FromInt(NewAmmo)));
	switch (SelectedWeapon->GetData()->AmmoUsageType)
	{
	case EAmmoUsageType::ClipAndReserve:
		ReserveAmmoText->SetText(FText::Format(FText::FromString("/ {0}"),
		                                       Player->GetInventoryComponent()->GetAmmo(
			                                       SelectedWeapon->GetData()->AmmoType)));
		break;
	case EAmmoUsageType::SinglePool:
		ReserveAmmoText->SetVisibility(ESlateVisibility::Collapsed);
		break;
	default:
		BTL_LOGC_ERROR(GetWorld(), "Invalid AmmoUsageType %d",
		               static_cast<int>(SelectedWeapon->GetData()->AmmoUsageType));
		break;
	}
}

void UHUDWidget::OnReserveAmmoUpdated()
{
	if (!SelectedWeapon)
		return;

	if (SelectedWeapon->GetData()->AmmoUsageType == EAmmoUsageType::ClipAndReserve)
	{
		ReserveAmmoText->SetText(FText::Format(FText::FromString("/ {0}"),
		                                       Player->GetInventoryComponent()->GetAmmo(
			                                       SelectedWeapon->GetData()->AmmoType)));
	}
}

void UHUDWidget::OnInventoryAmmoUpdated(FName AmmoType, int NewAmmo)
{
	OnReserveAmmoUpdated();
}

void UHUDWidget::AddNotification(FText Content, float Time)
{
	UNotificationWidget* Notification = CreateWidget<UNotificationWidget>(GetWorld(), NotificationWidgetClass);
	Notification->SetNotification(FUINotification(Content, Time));
	NotificationWidgets.Add(Notification);
	UVerticalBoxSlot* NewSlot = Cast<UVerticalBoxSlot>(PickupNotificationContainer->AddChild(Notification));
	NewSlot->SetPadding(FMargin(0, 0, 5, 5));
}

void UHUDWidget::UpdateInteractable(UInteractableComponent* Interactable)
{
	if (!Interactable)
	{
		InteractablePromptContainer->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	InteractablePromptContainer->SetVisibility(ESlateVisibility::Visible);
	InteractablePromptText->SetText(Interactable->Prompt);
}

void UHUDWidget::ShowPendingWeaponChoice(AWeapon* Weapon)
{
	Player->GetPlayerController()->ShowCursor();
	UWeaponChoiceUI* WeaponChoiceUI = CreateWidget<UWeaponChoiceUI>(GetWorld(), WeaponChoiceWidgetClass);
	WeaponChoiceUI->Show(Player, Weapon);
	WeaponChoiceUI->AddToViewport();
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Here, we'll iterate over all of our notifications and update their timer and alpha.
	// We would usually do this in the notification widget's tick function, but for some reason, that tick function isn't being called!
	// It's probably because the widget is somehow considered off the screen, even though it is clearly not!
	// Either way, while a bit messier, this method is probably more efficient too.
	for (int i = 0; i < NotificationWidgets.Num(); i++)
	{
		UNotificationWidget* Widget = NotificationWidgets[i];
		Widget->Time -= InDeltaTime;
		if (Widget->Time < 0.25f)
			Widget->SetAlpha(Widget->Time * 4);
		if (Widget->Time <= 0)
		{
			NotificationWidgets.RemoveAt(i);
			Widget->RemoveFromParent();
			i--;
		}
	}
}
