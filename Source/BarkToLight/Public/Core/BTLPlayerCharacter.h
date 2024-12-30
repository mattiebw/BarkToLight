// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "DamageSource.h"
#include "Character/PBPlayerCharacter.h"
#include "BTLPlayerCharacter.generated.h"

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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FVector GetLastKnownValidLocation() const { return LastKnownValidLocation; }

	UFUNCTION(BlueprintCallable)
	void ReturnToLastKnownLocation(float TimeToWait, bool bDoDamage = true);
	
	virtual AActor* GetDamageSource_Implementation() override;
	virtual FText GetDamageSourceName_Implementation() const override;
	virtual EDamageSourceType GetDamageSourceType_Implementation() const override;

protected:
	UFUNCTION()
	void OnBeginJump();
	UFUNCTION()
	void OnEndJump();
	UFUNCTION()
	void OnMove(const FInputActionValue& Value);
	UFUNCTION()
	void OnLook(const FInputActionValue& Value);
	UFUNCTION()
	void OnPrimaryFire(const FInputActionValue& Value);
	UFUNCTION()
	void OnSecondaryFire(const FInputActionValue& Value);
	UFUNCTION()
	void OnInteract(const FInputActionValue& Value);
	UFUNCTION()
	void OnBeginCrouchInput();
	UFUNCTION()
	void OnEndCrouchInput();
	
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext = nullptr;

	constexpr static float KnownLocationTimer = 0.25f;
	// The last known valid (grounded) location of the player character. Set every KnownLocationTimer seconds.
	// We can use this to return to a valid location if we fall of the map.
	UPROPERTY(BlueprintReadOnly)
	FVector LastKnownValidLocation;
	// The players location the last time we checked (every KnownLocationTimer seconds). If we're still grounded next
	// time that we check, it'll become the new LastKnownValidLocation.
	UPROPERTY(BlueprintReadOnly)
	FVector PendingKnownValidLocation;
	// Timer for checking the players location. We don't use Unreal's timer system as this is a very simple check,
	// and I've previously had problems with timers not firing when they should, and then not firing at all.
	float CurrentKnownLocationTimer = 0.25f;
};
