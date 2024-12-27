#include "BarkToLightEditorLog.h"

#if WITH_EDITOR
	#include "MessageLogModule.h"
#endif

DEFINE_LOG_CATEGORY(LogBarkToLightEditor);

void InitBarkToLightEditorLog()
{
#if WITH_EDITOR
	FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
	FMessageLogInitializationOptions InitOptions;
	InitOptions.bShowPages = true;
	InitOptions.bAllowClear = true;
	InitOptions.bShowFilters = true;
	MessageLogModule.RegisterLogListing("BarkToLightEditor", NSLOCTEXT("BarkToLightEditor", "BarkToLightEditorLogLabel", "Bark To Light Editor"), InitOptions);
#endif
}

void DeinitBarkToLightEditorLog()
{
#if WITH_EDITOR
	if (FModuleManager::Get().IsModuleLoaded("MessageLog"))
	{
		// Unregister message log
		FMessageLogModule& MessageLogModule = FModuleManager::GetModuleChecked<FMessageLogModule>("MessageLog");
		MessageLogModule.UnregisterLogListing("BarkToLightEditor");
	}
#endif
}
