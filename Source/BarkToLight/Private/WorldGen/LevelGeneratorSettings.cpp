// copyright lolol

#include "WorldGen/LevelGeneratorSettings.h"

#include "WorldGen/RoomFactory.h"

ULevelGeneratorSettings::ULevelGeneratorSettings()
{
	RoomFactoryClass = URoomFactory::StaticClass();
}

bool ULevelGeneratorSettings::Validate(FString& ErrorMessage, int MinRooms, int MaxRooms)
{
	// First, do we have any rooms at all?
	if (Rooms.IsEmpty())
	{
		ErrorMessage = "No rooms provided!";
		return false;
	}

	// Are the minimum and maximum counts valid?
	int MinRequiredRooms = 0;
	int MaxPossibleRooms = 0;
	for (int i = 0; i < Rooms.Num(); i++)
	{
		FRoomInfo& Room = Rooms[i];
		MinRequiredRooms += Room.MinimumCount;
		MaxPossibleRooms += Room.MaximumCount;

		if (Room.MinimumCount < 0)
		{
			ErrorMessage = FString::Printf(
				TEXT("Room with index %d has a negative minimum count."), i);
			return false;
		}

		if (Room.MaximumCount < Room.MinimumCount)
		{
			ErrorMessage = FString::Printf(
				TEXT("Room with index %d has a higher minimum count (%d) than maximum count (%d)!"), i, Room.MinimumCount,
				Room.MaximumCount);
			return false;
		}
	}

	// Do we have enough rooms?
	if (MaxRooms < MinRequiredRooms)
	{
		ErrorMessage = FString::Printf(TEXT("Room config requires a minimum of %d rooms, but only %d was allocated."),
		                               MinRequiredRooms, MaxRooms);
		return false;
	}

	// Or do we have too many?
	if (MinRooms > MaxPossibleRooms)
	{
		ErrorMessage = FString::Printf(TEXT("Level generator config requires a minimum of %d rooms, but only %d is possible due to room config."),
									   MinRooms, MaxPossibleRooms);
		return false;
	}

	return true;
}

void ULevelGeneratorSettings::CheckIsValid()
{
	FString ErrorMessage;
	bool bIsValid = Validate(ErrorMessage);

#if WITH_EDITOR
	if (!bIsValid)
	{
		FMessageDialog::Open(EAppMsgCategory::Error, EAppMsgType::Ok, FText::FromString(ErrorMessage),
		                     FText::FromString("Level Gen Settings are invalid!"));
	}
#endif
}
