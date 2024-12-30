// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class ABTLPlayerCharacter;

/**
 * A C++ interface for the HUD widget.
 */
UCLASS()
class BARKTOLIGHT_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "HUD")
	void OnPawnPossessed(ABTLPlayerCharacter* Character);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD")
	void FadeOut(FLinearColor To = FLinearColor::Black, float Time = 0.5f);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD")
	void FadeIn(FLinearColor From, float Time = 0.5f);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD")
	void FadeOutAndIn(FLinearColor To = FLinearColor::Black, float FadeOutTime = 0.5f, float HoldTime = 0.1f, float FadeInTime = 0.5f);
};
