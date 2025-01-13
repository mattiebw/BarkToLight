// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class ABTLPlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoUpdated, int, NewAmmo);

UCLASS()
class BARKTOLIGHT_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon", meta = (WorldContext = "WorldContextObject"))
	static AWeapon* CreateWeapon(UObject* WorldContextObject, UWeaponData* Data);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void InitialiseFromData(UWeaponData* NewData);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	FORCEINLINE UWeaponData* GetData() const { return Data; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFiring();

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void PreOnFire();
	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void OnFire();
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void PostOnFire();

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void OnStopFiring();

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void OnDryFire();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	bool CanFire() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void UseAmmo(int Amount);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	FORCEINLINE int GetLoadedAmmo() const { return  LoadedAmmo; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	FORCEINLINE int AmmoNeededToFillClip() const { return FMath::Max(0, Stats->GetClipSize() - LoadedAmmo); }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FORCEINLINE void GiveAmmo(int Amount) { LoadedAmmo = FMath::Min(Stats->GetClipSize(), LoadedAmmo + Amount); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	FORCEINLINE bool IsReloading() const { return ReloadTimeRemainingSeconds > 0; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload(int GivenAmmo);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	FORCEINLINE UWeaponStats* GetStats() { return Stats; }
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UWeaponStats> RequiredStatsClass = UWeaponStats::StaticClass();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon")
	ABTLPlayerCharacter* OwningPlayer;
	
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnAmmoUpdated OnAmmoUpdated;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	UWeaponData* Data;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon")
	UWeaponStats* Stats;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon")
	float LoadedAmmo = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon")
	float ReloadTimeRemainingSeconds = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon")
	int PendingAmmo = 0;
};
