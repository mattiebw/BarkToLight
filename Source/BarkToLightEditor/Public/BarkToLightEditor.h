#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FBarkToLightEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    UFUNCTION()
    void RegisterActorPlacements();
};
