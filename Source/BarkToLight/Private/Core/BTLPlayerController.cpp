// copyright lolol

#include "Core/BTLPlayerController.h"

#include "BarkToLightLog.h"
#include "Blueprint/UserWidget.h"
#include "Core/BTLCharacter.h"
#include "UI/HUDWidget.h"

void ABTLPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	BTLCharacter = Cast<ABTLCharacter>(InPawn);
	if (!BTLCharacter)
	{
		BTL_LOGC_ERROR_NOLOC(this, "BTL Player Controller possessed a pawn that is not a BTL Character!");
		return;
	}

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UHUDWidget>(this, HUDWidgetClass);
		HUDWidget->OnPawnPossessed(BTLCharacter);
		HUDWidget->AddToViewport();
	}
}
