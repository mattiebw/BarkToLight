// copyright lolol

using UnrealBuildTool;

public class BarkToLight : ModuleRules
{
	public BarkToLight(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "PBCharacterMovement", "UMG",
			"GeometryScriptingCore", "GeometryFramework", "AIModule", "NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		if (Target.bBuildEditor)
			PrivateDependencyModuleNames.AddRange(new string[] { "MessageLog", "UnrealEd" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
