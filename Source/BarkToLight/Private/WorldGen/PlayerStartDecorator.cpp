// copyright lolol

#include "WorldGen/PlayerStartDecorator.h"

#include "BarkToLightLog.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerStart.h"
#include "WorldGen/Room.h"
#include "WorldGen/RoomsLevelGenerator.h"

void UPlayerStartDecorator::Decorate()
{
	ARoom* RootRoom = Level->RootRoom.Actor;
	if (RootRoom == nullptr)
	{
		BTL_LOGC_ERROR(GetWorld(), "In PlayerStartDecorator::Decorate(), we couldn't find the root room.");
		return;
	}

	FVector Origin, Extents;
	RootRoom->GetActorBounds(true, Origin, Extents);

	APlayerStart* PlayerStart = GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), Origin,
	                                                                 FRotator::ZeroRotator);
	PlayerStart->AddActorWorldOffset(
		FVector(0, 0, Extents.Z + PlayerStart->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	Level->GetOtherGeneratedActors().Add(PlayerStart);

	Super::Decorate();
}
