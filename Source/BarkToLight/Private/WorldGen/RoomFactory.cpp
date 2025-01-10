// copyright lolol

#include "WorldGen/RoomFactory.h"

#include "BarkToLightLog.h"
#include "WorldGen/RoomsLevelGeneratorSettings.h"
#include "WorldGen/Room.h"
#include "WorldGen/RoomDecorator.h"

URoomFactory* URoomFactory::AddDecorator_Implementation(TSubclassOf<URoomDecorator> DecoratorClass)
{
	URoomDecorator* Decorator = NewObject<URoomDecorator>(this, DecoratorClass);
	Decorator->Room = Room;
	Decorator->Decorate();
	Decorator->ConditionalBeginDestroy();
	return this;
}

URoomFactory* URoomFactory::CreateRoom_Implementation(TSubclassOf<ARoom> RoomClass)
{
	if (Room)
	{
		BTL_LOGC_ERROR_NOLOC(GetWorld(), "RoomFactory::CreateRoom called with existing room. Call Finish() or Reset() first.");
		return this;
	}
	
	Room = GetWorld()->SpawnActor<ARoom>(RoomClass);
	
	return this;
}

URoomFactory* URoomFactory::AddDecoratorsFromSubclasses(TArray<TSubclassOf<URoomDecorator>> Decorators, float ChancePer)
{
	for (auto Decorator : Decorators)
	{
		if (FMath::FRand() <= ChancePer)
			AddDecorator(Decorator);
	}
	
	return this;
}

URoomFactory* URoomFactory::AddDecoratorsFromInfos(TArray<FRoomDecoratorInfo> Decorators)
{
	for (auto Decorator : Decorators)
	{
		if (FMath::FRand() <= Decorator.Chance)
			AddDecorator(Decorator.DecoratorClass);
	}
	
	return this;
}

URoomFactory* URoomFactory::Reset()
{
	Room = nullptr;
	return this;
}
