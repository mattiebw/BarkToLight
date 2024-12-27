#include "BarkToLightEditor.h"

#include "BarkToLightEditorLog.h"

#define LOCTEXT_NAMESPACE "BarkToLightEditorModule"

void FBarkToLightEditorModule::StartupModule()
{
    InitBarkToLightEditorLog();
    BTLED_LOG("Bark To Light Editor has started!");
}

void FBarkToLightEditorModule::ShutdownModule()
{
    BTLED_LOG_WARN("Bark To Light Editor is shutting down!");
    DeinitBarkToLightEditorLog();
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FBarkToLightEditorModule, BarkToLightEditor)
