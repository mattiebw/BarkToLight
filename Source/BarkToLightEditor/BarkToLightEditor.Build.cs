using UnrealBuildTool;

public class BarkToLightEditor : ModuleRules
{
    public BarkToLightEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "BarkToLight"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "MessageLog",
                "AssetDefinition",
                "EditorFramework",
                "UnrealEd",
            }
        );
    }
}