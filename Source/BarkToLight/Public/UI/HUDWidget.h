// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class ABTLCharacter;

/**
 * A C++ interface for the HUD widget.
 */
UCLASS()
class BARKTOLIGHT_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "HUD")
	void OnPawnPossessed(ABTLCharacter* Character);
};
