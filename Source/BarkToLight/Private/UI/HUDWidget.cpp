// copyright lolol

#include "UI/HUDWidget.h"

#include "Components/TextBlock.h"
#include "Core/BTLPlayerCharacter.h"
#include "Core/HealthComponent.h"

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

}

void UHUDWidget::OnHealthChanged_Implementation(float NewHealth, float HealthDifference)
{
	HealthText->SetText(FText::Format(FText::FromString("{0}"), NewHealth));
}
