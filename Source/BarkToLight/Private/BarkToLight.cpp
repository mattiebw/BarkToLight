// copyright lolol

#include "BarkToLight.h"

#include "BarkToLightLog.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FBarkToLightModule, BarkToLight, "Bark To Light" );

#define LOCTEXT_NAMESPACE "BarkToLight"

void FBarkToLightModule::StartupModule()
{
	InitBarkToLightLog();
	
	BTL_LOG("Started main Bark To Light module!");
}

void FBarkToLightModule::ShutdownModule()
{
	BTL_LOG_WARN("Shutting down main Bark To Light module.");
	
	DeinitBarkToLightLog();
}

#undef LOCTEXT_NAMESPACE
