// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/HUDWidget.h"
#include "BTLPlayerController.generated.h"

/**
 * The basic player controller.
 */
UCLASS()
class BARKTOLIGHT_API ABTLPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI")
	FORCEINLINE UHUDWidget* GetHUDWidget() const { return HUDWidget; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "BTL Player Controller")
	FORCEINLINE ABTLPlayerCharacter* GetBTLCharacter() const { return BTLCharacter; }
	
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UHUDWidget* HUDWidget;

	UPROPERTY(BlueprintReadOnly, Category = "BTL Player Controller")
	ABTLPlayerCharacter* BTLCharacter;
};
