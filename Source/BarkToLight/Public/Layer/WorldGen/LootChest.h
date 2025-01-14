// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Core/BTLPlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "LootChest.generated.h"

class UInventoryComponent;
class UInteractableComponent;

UCLASS()
class BARKTOLIGHT_API ALootChest : public AActor
{
	GENERATED_BODY()

public:
	ALootChest();

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnInteractedWith(ABTLPlayerCharacter* Player);

	virtual void Destroyed() override;
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Loot Chest")
	void OnLooted();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loot Chest")
	FORCEINLINE bool IsLooted() const { return bLooted; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Loot Chest")
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Loot Chest")
	bool bLooted = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Chest")
	UStaticMeshComponent* MeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Chest")
	UInventoryComponent* InventoryComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Chest")
	UInteractableComponent* InteractableComponent = nullptr;
};
