// copyright lolol

#include "Core/BTLPlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "BarkToLightLog.h"
#include "Character/PBPlayerMovement.h"
#include "Core/BTLPlayerController.h"
#include "Core/HealthComponent.h"
#include "Core/InteractableComponent.h"
#include "Core/PlayerStats.h"
#include "Core/Items/Item.h"
#include "Core/Items/ItemAppearance.h"
#include "Core/Items/PlayerUpgradeItem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/Weapon.h"

ABTLPlayerCharacter::ABTLPlayerCharacter(const FObjectInitializer& FObjectInitializer)
	: Super(FObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent    = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
}

void ABTLPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	LastKnownValidLocation = GetActorLocation();
	UseStats(Stats ? Stats : NewObject<UPlayerStats>(this)); // We have to *apply* our stats here.

	// We'll also give ourself a weapon!
	AWeapon* Weapon = AWeapon::CreateWeapon(this, StartingWeapon);
	InventoryComponent->GiveWeapon(Weapon);
	// For now, also give us 3 times the clip size worth of ammo. This shouldn't be hardcoded.
	InventoryComponent->GiveAmmo(Weapon->GetData()->AmmoType, Weapon->GetStats()->GetClipSize() * 3);
	SelectWeapon(0);
}

void ABTLPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update the known location.
	CurrentKnownLocationTimer -= DeltaTime;
	if (CurrentKnownLocationTimer <= 0)
	{
		// We're only looking for valid locations - which means we're on the ground.
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			LastKnownValidLocation    = PendingKnownValidLocation;
			PendingKnownValidLocation = GetActorLocation();
		}
		CurrentKnownLocationTimer = KnownLocationTimer;
	}

	// Check for interactables.
	// Trace to see if we are looking at an interactable object.
	FHitResult Hit;
	FVector    Start;
	FRotator   Dir;
	GetActorEyesViewPoint(Start, Dir);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, Start + Dir.Vector() * InteractDistance, ECC_Visibility);
	UInteractableComponent* Prev = TargetedInteractable;
	TargetedInteractable = Hit.GetActor() ? Hit.GetActor()->FindComponentByClass<UInteractableComponent>() : nullptr;
	// If this is a new interactable (or lack thereof), update the HUD.
	if (Prev != TargetedInteractable)
		PlayerController->GetHUDWidget()->UpdateInteractable(TargetedInteractable); // This function handles nulls.
}

void ABTLPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Input Mapping Context
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
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
		EnhancedInputComponent->BindAction(MouseMoveAction, ETriggerEvent::Triggered, this,
		                                   &ABTLPlayerCharacter::OnLook);

		// Primary Fire
		EnhancedInputComponent->BindAction(PrimaryFireAction, ETriggerEvent::Started, this,
		                                   &ABTLPlayerCharacter::OnBeginPrimaryFire);
		EnhancedInputComponent->BindAction(PrimaryFireAction, ETriggerEvent::Completed, this,
		                                   &ABTLPlayerCharacter::OnEndPrimaryFire);

		// Secondary Fire
		EnhancedInputComponent->BindAction(SecondaryFireAction, ETriggerEvent::Triggered, this,
		                                   &ABTLPlayerCharacter::OnSecondaryFire);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this,
		                                   &ABTLPlayerCharacter::OnInteract);

		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this,
		                                   &ABTLPlayerCharacter::OnBeginCrouchInput);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this,
		                                   &ABTLPlayerCharacter::OnEndCrouchInput);

		// Select Weapon
		EnhancedInputComponent->BindAction(SelectWeaponAction, ETriggerEvent::Triggered, this,
		                                   &ABTLPlayerCharacter::OnSelectWeapon);

		// Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this,
		                                   &ABTLPlayerCharacter::OnReload);
	}
	else
	{
		BTL_LOG_ERROR(
			"'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file.",
			*GetNameSafe(this));
	}
}

void ABTLPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PlayerController = Cast<ABTLPlayerController>(NewController);
	if (PlayerController == nullptr)
	{
		BTL_LOG_ERROR("Player character '%s' was possessed by a controller that is not a BTLPlayerController!",
		              *GetName());
		return;
	}

	// Update the HUD with our current weapon we have selected.
	PlayerController->GetHUDWidget()->OnWeaponSelected(InventoryComponent->GetWeapon(SelectedWeaponSlot));
}

void ABTLPlayerCharacter::UseStats(UPlayerStats* InStats)
{
	Stats = InStats;

	// We'll initialise our components with data from our stats, and also bind to the stat's callbacks so
	// we can update again when the stats change.
	
	HealthComponent->SetMaxHealth(Stats->GetHealth());
	HealthComponent->SetHealth(Stats->GetHealth());
	Stats->OnHealthChanged.AddDynamic(HealthComponent, &UHealthComponent::SetMaxHealth);

	GetMovementPtr()->MaxWalkSpeed = Stats->GetSpeed();
	Stats->OnSpeedChanged.AddDynamic(this, &ABTLPlayerCharacter::OnSpeedChanged);
	GetMovementPtr()->MaxWalkSpeedCrouched = Stats->GetSpeed() * Stats->GetCrouchSpeedMultiplier();
	Stats->OnCrouchSpeedMultiplierChanged.AddDynamic(this, &ABTLPlayerCharacter::OnCrouchSpeedMultiplierChanged);
	GetMovementPtr()->JumpZVelocity = Stats->GetJumpZ();
	Stats->OnJumpZChanged.AddDynamic(this, &ABTLPlayerCharacter::OnJumpZChanged);

	InventoryComponent->SetWeaponSlots(Stats->GetWeaponSlots());
	Stats->OnWeaponSlotsChanged.AddDynamic(this, &ABTLPlayerCharacter::OnWeaponSlotsChanged);
}

// -----------------------------
// --- Stat Change Callbacks ---
// -----------------------------
void ABTLPlayerCharacter::OnSpeedChanged(float NewValue)
{
	GetMovementPtr()->MaxWalkSpeed = NewValue;
}

void ABTLPlayerCharacter::OnCrouchSpeedMultiplierChanged(float NewValue)
{
	GetMovementPtr()->MaxWalkSpeedCrouched = Stats->GetSpeed() * NewValue;
}

void ABTLPlayerCharacter::OnJumpZChanged(float NewValue)
{
	GetMovementPtr()->JumpZVelocity = NewValue;
}

void ABTLPlayerCharacter::OnWeaponSlotsChanged(float NewValue)
{
	if (SelectedWeaponSlot >= NewValue)
	{
		InventoryComponent->GetWeapon(SelectedWeaponSlot)->StopFiring();
		SelectedWeaponSlot = 0;
	}

	TArray<AWeapon*> Culled = InventoryComponent->SetWeaponSlots(NewValue);
	for (AWeapon* Weapon : Culled)
	{
		// MW @todo: Drop the weapon
		Weapon->Destroy();
	}
}

// -----------------------------

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

void ABTLPlayerCharacter::SelectWeapon(int Slot)
{
	BTL_LOG("Selecting weapon slot %f", Slot);
	if (AWeapon* Weapon = InventoryComponent->GetWeapon(Slot))
	{
		Weapon->OwningPlayer = this;

		if (bFiring)
		{
			InventoryComponent->GetWeapon(SelectedWeaponSlot)->StopFiring();
			bFiring = false;
		}

		// MW @todo: Equip
		PlayerController->GetHUDWidget()->OnWeaponSelected(Weapon);
		SelectedWeaponSlot = Slot;
	}
}

void ABTLPlayerCharacter::SubmitPendingWeaponChoice(bool bKeepWeapon, int SlotToDiscard)
{
	AWeapon* TheWeapon = nullptr;
	PendingWeaponQueue.Dequeue(TheWeapon);

	if (!bKeepWeapon)
	{
		// If we're not keeping our new weapon, we should destroy it.
		TheWeapon->Destroy();
	}
	else
	{
		// Otherwise, destroy the previous weapon and replace it with the new one.
		InventoryComponent->GetWeapon(SlotToDiscard)->Destroy();
		InventoryComponent->SetWeapon(SlotToDiscard, TheWeapon);
	}

	// Check to see if there's another weapon in the queue.
	if (!PendingWeaponQueue.IsEmpty())
		PlayerController->GetHUDWidget()->ShowPendingWeaponChoice(*PendingWeaponQueue.Peek());
	else
		PlayerController->HideCursor();
}

void ABTLPlayerCharacter::LootInventory(UInventoryComponent* Inventory)
{
	// First, add the ammo.
	for (auto Ammo : Inventory->GetAmmoInventory())
	{
		InventoryComponent->GiveAmmo(Ammo.Key, Ammo.Value);
		PlayerController->GetHUDWidget()->AddNotification(
			FText::FromString(FString::Printf(TEXT("+%d %s"), Ammo.Value, *Ammo.Key.ToString())));
	}

	// Now, we add the weapons.
	for (AWeapon* Weapon : Inventory->GetWeapons())
	{
		if (InventoryComponent->GetFreeWeaponSlots() == 0)
		{
			PendingWeaponQueue.Enqueue(Weapon);
		}
		else
		{
			InventoryComponent->GiveWeapon(Weapon);
			PlayerController->GetHUDWidget()->AddNotification(
				FText::Format(FText::FromString(TEXT("+{0}")), Weapon->GetData()->Name));
		}
	}

	// Finally, try and add any items.
	for (UItem* Item : Inventory->GetItems())
	{
		InventoryComponent->GiveItem(Item);
		if (UPlayerUpgradeItem* PlayerUpgrade = Cast<UPlayerUpgradeItem>(Item))
			PlayerUpgrade->Execute_Equip(PlayerUpgrade, this);

		PlayerController->GetHUDWidget()->AddNotification(
			FText::Format(FText::FromString(TEXT("+{0}")), Item->Appearance->Name));
	}

	Inventory->Clear();

	if (!PendingWeaponQueue.IsEmpty())
	{
		PlayerController->GetHUDWidget()->ShowPendingWeaponChoice(*PendingWeaponQueue.Peek());
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

void ABTLPlayerCharacter::OnBeginPrimaryFire(const FInputActionValue& Value)
{
	if (AWeapon* Weapon = InventoryComponent->GetWeapon(SelectedWeaponSlot))
	{
		Weapon->Fire();
	}
}

void ABTLPlayerCharacter::OnEndPrimaryFire(const FInputActionValue& Value)
{
	if (AWeapon* Weapon = InventoryComponent->GetWeapon(SelectedWeaponSlot))
	{
		Weapon->StopFiring();
	}
}

void ABTLPlayerCharacter::OnSecondaryFire(const FInputActionValue& Value)
{
}

void ABTLPlayerCharacter::OnInteract(const FInputActionValue& Value)
{
	if (TargetedInteractable)
		TargetedInteractable->Interact(this);
}

void ABTLPlayerCharacter::OnBeginCrouchInput()
{
	Crouch();
}

void ABTLPlayerCharacter::OnEndCrouchInput()
{
	UnCrouch();
}

void ABTLPlayerCharacter::OnSelectWeapon(const FInputActionValue& Value)
{
	float Slot = Value.Get<float>();
	SelectWeapon(static_cast<int>(Slot - 1));
}

void ABTLPlayerCharacter::OnReload()
{
	AWeapon* Weapon = InventoryComponent->GetWeapon(SelectedWeaponSlot);
	if (!Weapon)
		return;

	if (Weapon->IsReloading())
		return;
	int AmmoNeeded = Weapon->AmmoNeededToFillClip();
	if (AmmoNeeded <= 0)
		return;
	int AmmoObtained = InventoryComponent->TakeAmmo(Weapon->GetData()->AmmoType, AmmoNeeded);
	if (AmmoObtained > 0)
	{
		Weapon->Reload(AmmoObtained);
	}
}
