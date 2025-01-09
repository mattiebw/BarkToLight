// copyright lolol

#include "WorldGen/RoomFactory.h"

URoomFactory* URoomFactory::AddDecorator_Implementation(TSubclassOf<URoomDecorator> DecoratorClass)
{
	return this;
}

URoomFactory* URoomFactory::CreateRoom_Implementation(TSubclassOf<ARoom> RoomClass)
{
	return this;
}

URoomFactory* URoomFactory::Reset()
{
	Room = nullptr;
	return this;
}
