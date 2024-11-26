#include "BarkToLightLog.h"

#if WITH_EDITOR
	#include "MessageLogModule.h"
#endif

DEFINE_LOG_CATEGORY(LogBarkToLight);

void InitBarkToLightLog()
{
#if WITH_EDITOR
	FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
	FMessageLogInitializationOptions InitOptions;
	InitOptions.bShowPages = true;
	InitOptions.bAllowClear = true;
	InitOptions.bShowFilters = true;
	MessageLogModule.RegisterLogListing("BarkToLight", NSLOCTEXT("BarkToLight", "BarkToLightLogLabel", "Bark To Light"), InitOptions);
#endif
}

void DeinitBarkToLightLog()
{
#if WITH_EDITOR
	if (FModuleManager::Get().IsModuleLoaded("MessageLog"))
	{
		// Unregister message log
		FMessageLogModule& MessageLogModule = FModuleManager::GetModuleChecked<FMessageLogModule>("MessageLog");
		MessageLogModule.UnregisterLogListing("BarkToLight");
	}
#endif
}
