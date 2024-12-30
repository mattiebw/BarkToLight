#include "BarkToLightEditor.h"

#include "BarkToLightEditorLog.h"

#include "WorldGen/BTLGeneratedMeshActors.h"
#include "IPlacementModeModule.h"

#define LOCTEXT_NAMESPACE "BarkToLightEditorModule"

void FBarkToLightEditorModule::StartupModule()
{
    InitBarkToLightEditorLog();
    BTLED_LOG("Bark To Light Editor has started!");

    // We have to wait until post engine init to register our placement mode items, as the placement mode module
    // is not available until then, and we'll crash if we try to access it before it's ready.
    FCoreDelegates::OnPostEngineInit.AddRaw(this, &FBarkToLightEditorModule::RegisterActorPlacements);
}

void FBarkToLightEditorModule::RegisterActorPlacements()
{
    // Create a custom category and register it
    const FPlacementCategoryInfo Info(
        INVTEXT("Bark To Light"),
        FSlateIcon(FAppStyle::GetAppStyleSetName(), "PlacementBrowser.Icons.Basic"),
        "BTL",
        TEXT("BTL"),
        100
    );
 
    IPlacementModeModule& PlacementModeModule = IPlacementModeModule::Get();
    PlacementModeModule.RegisterPlacementCategory(Info);
 
    PlacementModeModule.RegisterPlaceableItem(Info.UniqueHandle,
        MakeShared<FPlaceableItem>(nullptr, FAssetData(ATreacheryPlatformGeneratedMeshActor::StaticClass())));
}

void FBarkToLightEditorModule::ShutdownModule()
{
    BTLED_LOG_WARN("Bark To Light Editor is shutting down!");
    DeinitBarkToLightEditorLog();

    if (IPlacementModeModule::IsAvailable())
    {
        IPlacementModeModule::Get().UnregisterPlacementCategory("BTL");
    }
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FBarkToLightEditorModule, BarkToLightEditor)
