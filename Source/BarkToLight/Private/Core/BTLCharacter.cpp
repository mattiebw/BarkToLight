// copyright lolol

#include "Core/BTLCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "BarkToLightLog.h"

ABTLCharacter::ABTLCharacter(const FObjectInitializer& FObjectInitializer)
	: Super(FObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABTLCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABTLCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABTLCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this,
		                                   &ABTLCharacter::OnBeginJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
		                                   &ABTLCharacter::OnEndJump);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABTLCharacter::OnMove);

		// Looking
		EnhancedInputComponent->BindAction(MouseMoveAction, ETriggerEvent::Triggered, this, &ABTLCharacter::OnLook);

		// Primary Fire
		EnhancedInputComponent->BindAction(PrimaryFireAction, ETriggerEvent::Triggered, this, &ABTLCharacter::OnPrimaryFire);
		
		// Secondary Fire
		EnhancedInputComponent->BindAction(SecondaryFireAction, ETriggerEvent::Triggered, this, &ABTLCharacter::OnSecondaryFire);
		
		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ABTLCharacter::OnInteract);
	}
	else
	{
		BTL_LOG_ERROR(
			"'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file.",
			*GetNameSafe(this));
	}
}

void ABTLCharacter::OnBeginJump()
{
	Jump();
}

void ABTLCharacter::OnEndJump()
{
	StopJumping();
}

void ABTLCharacter::OnMove(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
    	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
    	AddMovementInput(GetActorRightVector(), MovementVector.X);
    }
}

void ABTLCharacter::OnLook(const FInputActionValue& Value)
{
	FVector2D LookDir = Value.Get<FVector2D>();
	AddControllerYawInput(LookDir.X);
	AddControllerPitchInput(LookDir.Y);
}

void ABTLCharacter::OnPrimaryFire(const FInputActionValue& Value)
{
}

void ABTLCharacter::OnSecondaryFire(const FInputActionValue& Value)
{
}

void ABTLCharacter::OnInteract(const FInputActionValue& Value)
{
}
