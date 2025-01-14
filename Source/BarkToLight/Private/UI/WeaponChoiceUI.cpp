// copyright lolol

#include "UI/WeaponChoiceUI.h"

#include "BarkToLightLog.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Core/BTLPlayerCharacter.h"
#include "Core/BTLPlayerController.h"
#include "Weapon/Weapon.h"

void UWeaponUI::SetWeapon(AWeapon* InWeapon)
{
	if (!InWeapon)
	{
		WeaponName->SetText(FText::FromString("No Weapon"));
	}

	Weapon = InWeapon;
	WeaponName->SetText(InWeapon->GetData()->Name);
	if (InWeapon->GetStats()->HasAnyUpgrades())
		WeaponUpgrades->SetText(FText::FromString(InWeapon->GetStats()->GetUpgradeString()));
	else
		WeaponUpgrades->SetText(FText::FromString("No Upgrades"));
}

void UWeaponUI::ChoiceMade()
{
	OnWeaponChoice.Broadcast(Weapon);
}

void UWeaponChoiceUI::Show(ABTLPlayerCharacter* InPlayer, AWeapon* InNewWeapon)
{
	Player = InPlayer;

	auto& CurrentWeapons = InPlayer->GetInventoryComponent()->GetWeapons();
	for (AWeapon* Weapon : CurrentWeapons)
	{
		UWeaponUI* WeaponUI = CreateWidget<UWeaponUI>(GetWorld(), WeaponUIWidgetClass);
		WeaponUI->SetWeapon(Weapon);
		UHorizontalBoxSlot* WeaponUISlot = Cast<UHorizontalBoxSlot>(CurrentWeaponList->AddChild(WeaponUI));
		WeaponUISlot->SetPadding(FMargin(20, 5));
		WeaponUI->OnWeaponChoice.AddUniqueDynamic(this, &UWeaponChoiceUI::OnWeaponSelected);
	}

	NewWeaponUI->SetWeapon(InNewWeapon);
	NewWeaponUI->OnWeaponChoice.AddUniqueDynamic(this, &UWeaponChoiceUI::OnWeaponSelected);

	NewWeapon = InNewWeapon;
}

void UWeaponChoiceUI::OnWeaponSelected(AWeapon* Weapon)
{
	if (!Weapon)
	{
		BTL_LOG_ERROR("In UWeaponChoiceUI::OnWeaponSelected(), Weapon is null!");
		if (Player)
			Player->GetPlayerController()->HideCursor();
		RemoveFromParent();
		return;
	}

	if (Weapon == NewWeapon)
	{
		// The player has selected the new weapon to discard, so we don't keep it.
		Player->SubmitPendingWeaponChoice(false, -1);
	} else
	{
		auto& CurrentWeapons = Player->GetInventoryComponent()->GetWeapons();
		for (int i = 0; i < CurrentWeapons.Num(); i++)
		{
			if (CurrentWeapons[i] == Weapon)
			{
				Player->SubmitPendingWeaponChoice(true, i);
				break;
			}
		}
	}

	RemoveFromParent();
}
