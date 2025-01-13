// copyright lolol

#include "Weapon/Weapon.h"

#include "BarkToLightLog.h"
#include "Core/BTLPlayerCharacter.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (Data != nullptr)
		InitialiseFromData(Data);
}

void AWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (OwningPlayer)
	{
		SetActorLocation(OwningPlayer->GetActorLocation() + (OwningPlayer->GetBaseAimRotation().RotateVector(OwningPlayer->WeaponLocation)));
	}
	
	if (ReloadTimeRemainingSeconds > 0)
	{
		ReloadTimeRemainingSeconds = FMath::Max(0.0f, ReloadTimeRemainingSeconds - DeltaSeconds);
		if (ReloadTimeRemainingSeconds == 0)
		{
			LoadedAmmo = FMath::Min(Stats->GetClipSize(), LoadedAmmo + PendingAmmo);
			PendingAmmo = 0;
			OnAmmoUpdated.Broadcast(LoadedAmmo);
		}
	}
}

AWeapon* AWeapon::CreateWeapon(UObject* WorldContextObject, UWeaponData* Data)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
		return nullptr;

	AWeapon* Weapon = World->SpawnActorDeferred<AWeapon>(Data->WeaponSubclass, FTransform::Identity);
	Weapon->Data = Data;
	Weapon->FinishSpawning(FTransform::Identity);
	return Weapon;
}

void AWeapon::InitialiseFromData(UWeaponData* NewData)
{
	Data = NewData;
	
	Stats = DuplicateObject(Data->DefaultStats, this);
	if (!Stats->IsA(RequiredStatsClass))
		BTL_LOGC_ERROR(GetWorld(), "In Weapon %s, Stats class %s does not extend the required stats class %s", *GetName(), *Stats->GetClass()->GetName(), *RequiredStatsClass->GetName());
}

void AWeapon::Fire()
{
	if (!CanFire())
		return;

	switch (Data->AmmoUsageType)
	{
	case EAmmoUsageType::ClipAndReserve:
	case EAmmoUsageType::SinglePool:
		UseAmmo(1);
		break;
	case EAmmoUsageType::Charge:
		// MW @todo
		break;
	case EAmmoUsageType::InfiniteAmmo:
		break;
	}
	
	PreOnFire();
	OnFire();
	PostOnFire();
}

void AWeapon::StopFiring()
{
	OnStopFiring();
}

void AWeapon::UseAmmo(int Amount)
{
	LoadedAmmo -= Amount;
	if (LoadedAmmo < 0)
		LoadedAmmo = 0;
	OnAmmoUpdated.Broadcast(LoadedAmmo);
}

void AWeapon::Reload(int GivenAmmo)
{
	if (LoadedAmmo == Stats->GetClipSize()
		|| Data->AmmoUsageType == EAmmoUsageType::InfiniteAmmo
		|| Data->AmmoUsageType == EAmmoUsageType::Charge)
	{
		return;
	}
	
	ReloadTimeRemainingSeconds = Stats->GetReloadTimeSeconds();
	PendingAmmo = GivenAmmo;
}

void AWeapon::OnStopFiring_Implementation()
{
	// MW @todo: Implement for continuous fire weapons.
}

bool AWeapon::CanFire_Implementation() const
{
	if (ReloadTimeRemainingSeconds > 0)
		return false;
	
	switch (Data->AmmoUsageType)
	{
	case EAmmoUsageType::ClipAndReserve:
	case EAmmoUsageType::SinglePool:
		return LoadedAmmo > 0;
		break;
	case EAmmoUsageType::Charge:
		// MW @todo
		return true;
	case EAmmoUsageType::InfiniteAmmo:
		return true;
	}

	BTL_LOGC_ERROR(GetWorld(), "Invalid AmmoUsageType %d", static_cast<int>(Data->AmmoUsageType));
	return false;
}

void AWeapon::OnFire_Implementation()
{
	BTL_LOGC_ERROR(GetWorld(), "Weapon %s does not implement OnFire_Implementation!", *GetName());
}
