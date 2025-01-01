// copyright lolol

#include "Weapon/Weapon.h"

#include "Core/BTLPlayerCharacter.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (Data != nullptr)
		InitialiseFromData(Data);
}

void AWeapon::InitialiseFromData(UWeaponData* NewData)
{
	Data = NewData;
	
	Stats = DuplicateObject(Data->DefaultStats, this);
}

void AWeapon::Fire()
{
	PreOnFire();
	OnFire();
	PostOnFire();
}

void AWeapon::OnFire_Implementation()
{
	FVector Direction = OwningPlayer->GetBaseAimRotation().Vector();
}
