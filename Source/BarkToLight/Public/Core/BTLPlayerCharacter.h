// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "DamageSource.h"
#include "InventoryComponent.h"
#include "Character/PBPlayerCharacter.h"
#include "BTLPlayerCharacter.generated.h"

class UWeaponData;
class UInteractableComponent;
class ABTLPlayerController;
class UPlayerStats;
class UHealthComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class BARKTOLIGHT_API ABTLPlayerCharacter : public APBPlayerCharacter, public IDamageSource
{
	GENERATED_BODY()

public:
	ABTLPlayerCharacter(const FObjectInitializer& FObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	// --- Player Stats ---
	UFUNCTION(BlueprintCallable, Category = "Player")
	void UseStats(UPlayerStats* InStats);
	UFUNCTION()
	void OnSpeedChanged(float NewValue);
	UFUNCTION()
	void OnCrouchSpeedMultiplierChanged(float NewValue);
	UFUNCTION()
	void OnJumpZChanged(float NewValue);
	UFUNCTION()
	void OnWeaponSlotsChanged(float NewValue);
	// --------------------

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player")
	FORCEINLINE FVector GetLastKnownValidLocation() const { return LastKnownValidLocation; }
	UFUNCTION(BlueprintCallable, Category = "Player")
	void ReturnToLastKnownLocation(float TimeToWait, bool bDoDamage = true);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player")
	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player")
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player")
	FORCEINLINE UPlayerStats* GetStats() const { return Stats; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player")
	FORCEINLINE ABTLPlayerController* GetPlayerController() const { return PlayerController; }

	UFUNCTION(BlueprintCallable, Category = "Player")
	void SelectWeapon(int Slot);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player", meta = (MakeEditWidget))
	FVector WeaponLocation;

	UFUNCTION(BlueprintCallable, Category = "Player")
	void SubmitPendingWeaponChoice(bool bKeepWeapon, int SlotToDiscard);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void LootInventory(UInventoryComponent* Inventory);

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	bool bHasSavedDog = false;
	
	// --- IDamageSource ---
	virtual AActor*           GetDamageSource_Implementation() override;
	virtual FText             GetDamageSourceName_Implementation() const override;
	virtual EDamageSourceType GetDamageSourceType_Implementation() const override;
	// ---------------------

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Player")
	UPlayerStats* Stats;
	
	UPROPERTY(BlueprintReadOnly, Category = "Player")
	ABTLPlayerController* PlayerController;

	/// --- Input Data and Functions ---
	UFUNCTION()
	void OnBeginJump();
	UFUNCTION()
	void OnEndJump();
	UFUNCTION()
	void OnMove(const FInputActionValue& Value);
	UFUNCTION()
	void OnLook(const FInputActionValue& Value);
	UFUNCTION()
	void OnBeginPrimaryFire(const FInputActionValue& Value);
	UFUNCTION()
	void OnEndPrimaryFire(const FInputActionValue& Value);
	UFUNCTION()
	void OnSecondaryFire(const FInputActionValue& Value);
	UFUNCTION()
	void OnInteract(const FInputActionValue& Value);
	UFUNCTION()
	void OnBeginCrouchInput();
	UFUNCTION()
	void OnEndCrouchInput();
	UFUNCTION()
	void OnSelectWeapon(const FInputActionValue& Value);
	UFUNCTION()
	void OnReload();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MouseMoveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PrimaryFireAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SecondaryFireAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CrouchAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SelectWeaponAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ReloadAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext = nullptr;
	// --------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	int SelectedWeaponSlot = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	UWeaponData* StartingWeapon;
	
	TQueue<AWeapon*> PendingWeaponQueue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	bool bFiring = false;

	constexpr static float InteractDistance = 400.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Player")
	UInteractableComponent* TargetedInteractable = nullptr;

	// Constant declaring the time between checks for the players location.
	constexpr static float KnownLocationTimer = 0.25f;
	// The last known valid (grounded) location of the player character. Set every KnownLocationTimer seconds.
	// We can use this to return to a valid location if we fall of the map.
	UPROPERTY(BlueprintReadOnly, Category = "Player")
	FVector LastKnownValidLocation;
	// The players location the last time we checked (every KnownLocationTimer seconds). If we're still grounded next
	// time that we check, it'll become the new LastKnownValidLocation.
	UPROPERTY(BlueprintReadOnly, Category = "Player")
	FVector PendingKnownValidLocation;
	// Timer for checking the players location. We don't use Unreal's timer system as this is a very simple check,
	// and I've previously had problems with timers not firing when they should, and then not firing at all.
	float CurrentKnownLocationTimer = 0.25f;
};
