// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define ECC_Weapon ECC_GameTraceChannel1
#define ECC_CurrentRoom ECC_GameTraceChannel2

class BARKTOLIGHT_API FBarkToLightModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
