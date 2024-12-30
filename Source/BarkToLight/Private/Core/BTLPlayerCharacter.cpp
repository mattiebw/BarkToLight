// copyright lolol

#include "Core/BTLPlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "BarkToLightLog.h"
#include "Core/BTLPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

ABTLPlayerCharacter::ABTLPlayerCharacter(const FObjectInitializer& FObjectInitializer)
	: Super(FObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABTLPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	LastKnownValidLocation = GetActorLocation();
}

void ABTLPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentKnownLocationTimer -= DeltaTime;
	if (CurrentKnownLocationTimer <= 0)
	{
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			LastKnownValidLocation = PendingKnownValidLocation;
			PendingKnownValidLocation = GetActorLocation();
		}
		CurrentKnownLocationTimer = KnownLocationTimer;
	}
}

void ABTLPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
		                                   &ABTLPlayerCharacter::OnBeginJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
		                                   &ABTLPlayerCharacter::OnEndJump);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABTLPlayerCharacter::OnMove);

		// Looking
		EnhancedInputComponent->BindAction(MouseMoveAction, ETriggerEvent::Triggered, this, &ABTLPlayerCharacter::OnLook);

		// Primary Fire
		EnhancedInputComponent->BindAction(PrimaryFireAction, ETriggerEvent::Triggered, this,
		                                   &ABTLPlayerCharacter::OnPrimaryFire);

		// Secondary Fire
		EnhancedInputComponent->BindAction(SecondaryFireAction, ETriggerEvent::Triggered, this,
		                                   &ABTLPlayerCharacter::OnSecondaryFire);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ABTLPlayerCharacter::OnInteract);

		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this,
		                                   &ABTLPlayerCharacter::OnBeginCrouchInput);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this,
		                                   &ABTLPlayerCharacter::OnEndCrouchInput);
	}
	else
	{
		BTL_LOG_ERROR(
			"'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file.",
			*GetNameSafe(this));
	}
}

void ABTLPlayerCharacter::ReturnToLastKnownLocation(float TimeToWait, bool bDoDamage)
{
	// MW @todo: Damage

	if (ABTLPlayerController* BTLController = Cast<ABTLPlayerController>(GetController()))
	{
		BTLController->GetHUDWidget()->FadeOutAndIn(FLinearColor::Black, TimeToWait, 0.1f, 0.2f);

		// Wait for the fade to complete before moving the player.
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this]()
		{
			GetCharacterMovement()->Velocity = FVector::ZeroVector;
			SetActorLocation(LastKnownValidLocation, false, nullptr, ETeleportType::ResetPhysics);
		}, TimeToWait, false);
	}
}

AActor* ABTLPlayerCharacter::GetDamageSource_Implementation()
{
	return this;
}

FText ABTLPlayerCharacter::GetDamageSourceName_Implementation() const
{
	return FText::FromString("Player");
}

EDamageSourceType ABTLPlayerCharacter::GetDamageSourceType_Implementation() const
{
	return EDamageSourceType::Player;
}

void ABTLPlayerCharacter::OnBeginJump()
{
	Jump();
}

void ABTLPlayerCharacter::OnEndJump()
{
	StopJumping();
}

void ABTLPlayerCharacter::OnMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ABTLPlayerCharacter::OnLook(const FInputActionValue& Value)
{
	FVector2D LookDir = Value.Get<FVector2D>();
	AddControllerYawInput(LookDir.X);
	AddControllerPitchInput(LookDir.Y);
}

void ABTLPlayerCharacter::OnPrimaryFire(const FInputActionValue& Value)
{
}

void ABTLPlayerCharacter::OnSecondaryFire(const FInputActionValue& Value)
{
}

void ABTLPlayerCharacter::OnInteract(const FInputActionValue& Value)
{
}

void ABTLPlayerCharacter::OnBeginCrouchInput()
{
	Crouch();
}

void ABTLPlayerCharacter::OnEndCrouchInput()
{
	UnCrouch();
}
