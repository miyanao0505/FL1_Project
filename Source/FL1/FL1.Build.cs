// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FL1 : ModuleRules
{
	public FL1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
            "NavigationSystem",
            "StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"FL1",
			"FL1/Variant_Platforming",
			"FL1/Variant_Platforming/Animation",
			"FL1/Variant_Combat",
			"FL1/Variant_Combat/AI",
			"FL1/Variant_Combat/Animation",
			"FL1/Variant_Combat/Gameplay",
			"FL1/Variant_Combat/Interfaces",
			"FL1/Variant_Combat/UI",
			"FL1/Variant_SideScrolling",
			"FL1/Variant_SideScrolling/AI",
			"FL1/Variant_SideScrolling/Gameplay",
			"FL1/Variant_SideScrolling/Interfaces",
			"FL1/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
