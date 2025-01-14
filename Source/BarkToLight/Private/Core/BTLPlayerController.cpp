// copyright lolol

#include "Core/BTLPlayerController.h"

#include "BarkToLightLog.h"
#include "Blueprint/UserWidget.h"
#include "Core/BTLPlayerCharacter.h"
#include "UI/HUDWidget.h"

void ABTLPlayerController::OnPossess(APawn* InPawn)
{
	BTLCharacter = Cast<ABTLPlayerCharacter>(InPawn);
	if (!BTLCharacter)
	{
		BTL_LOGC_ERROR_NOLOC(this, "BTL Player Controller possessed a pawn that is not a BTL Character!");
		return;
	}

	// Setup our HUD BEFORE calling Super::OnPossess, as it is used in PossessedBy in the character.
	
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
		HUDWidget->OnPawnPossessed(BTLCharacter);
		HUDWidget->AddToViewport();
	}

	Super::OnPossess(InPawn);
	
	PlayerCameraManager->SetFOV(110);
}

void ABTLPlayerController::ShowCursor()
{
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}

void ABTLPlayerController::HideCursor()
{
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}
