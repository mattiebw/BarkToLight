// copyright lolol

#include "WorldGen/EnemyEncounterDecorator.h"

#include "Enemy/EnemyEncounterVolume.h"
#include "WorldGen/Room.h"

void UEnemyEncounterDecorator::Decorate_Implementation()
{
	float Difficulty = Room->Difficulty;

	float TotalWeight = 0;
	for (FEnemyType& EnemyType : EnemyTypes)
	{
		EnemyType.MinRoll = TotalWeight;
		TotalWeight += EnemyType.Weight;
		EnemyType.MaxRoll = TotalWeight;
	}

	TMap<UEnemyData*, int> Spawns;
	
	while (Difficulty > 0)
	{
		float Roll = FMath::FRandRange(0, TotalWeight);
		for (const FEnemyType& EnemyType : EnemyTypes)
		{
			if (Roll < EnemyType.MaxRoll && Roll > EnemyType.MinRoll)
			{
				Difficulty -= EnemyType.DifficultyCost;
				if (!Spawns.Contains(EnemyType.EnemyData))
					Spawns.Add(EnemyType.EnemyData, 1);
				else
					Spawns[EnemyType.EnemyData]++;
				break;
			}
		}	
	}

	AEnemyEncounterVolume* EncounterVolume = GetWorld()->SpawnActor<AEnemyEncounterVolume>();
	FVector Center, Extent;
	Room->GetActorBounds(true, Center, Extent);
	EncounterVolume->SetCenterAndExtent(Center, Extent);
	EncounterVolume->Spawns = Spawns;
	Room->Parts.Add(EncounterVolume);
}
