// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class ABTLPlayerCharacter;

UCLASS()
class BARKTOLIGHT_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void InitialiseFromData(UWeaponData* NewData);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	FORCEINLINE UWeaponData* GetData() const { return Data; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void PreOnFire();
	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void OnFire();
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void PostOnFire();
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	ABTLPlayerCharacter* OwningPlayer;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	UWeaponData* Data;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	UWeaponStats* Stats;
};
