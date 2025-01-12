// copyright lolol

#include "UI/HUDWidget.h"

#include "AnimationEditorViewportClient.h"
#include "BarkToLightLog.h"
#include "Components/TextBlock.h"
#include "Core/BTLPlayerCharacter.h"
#include "Core/HealthComponent.h"
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
