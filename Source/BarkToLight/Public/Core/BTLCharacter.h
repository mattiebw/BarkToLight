// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Character/PBPlayerCharacter.h"
#include "BTLCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class BARKTOLIGHT_API ABTLCharacter : public APBPlayerCharacter
{
	GENERATED_BODY()

public:
	ABTLCharacter(const FObjectInitializer& FObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MouseMoveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* PrimaryFireAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SecondaryFireAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InteractAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* CrouchAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext = nullptr;
};
