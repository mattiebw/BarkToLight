// copyright lolol

#include "WorldGen/EnemyEncounterDecorator.h"

#include "WorldGen/Room.h"

void UEnemyEncounterDecorator::Decorate_Implementation()
{
	float Difficulty = Room->Difficulty;

	float TotalWeight = 0;
	for (const FEnemyType& EnemyType : EnemyTypes)
	{
		TotalWeight += EnemyType.Weight;
	}
	
	while (Difficulty > 0)
	{
		float Roll = FMath::FRandRange(0, TotalWeight);
		for (const FEnemyType& EnemyType : EnemyTypes)
		{
			if (Roll < EnemyType.Weight)
			{
				Difficulty -= EnemyType.DifficultyCost;
				break;
			}
			Roll -= EnemyType.Weight;
		}	
	}
}
