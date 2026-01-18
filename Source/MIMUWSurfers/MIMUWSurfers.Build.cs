// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MIMUWSurfers : ModuleRules
{
	public MIMUWSurfers(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"SlateCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"MIMUWSurfers",
			"MIMUWSurfers/Variant_Platforming",
			"MIMUWSurfers/Variant_Platforming/Animation",
			"MIMUWSurfers/Variant_Combat",
			"MIMUWSurfers/Variant_Combat/AI",
			"MIMUWSurfers/Variant_Combat/Animation",
			"MIMUWSurfers/Variant_Combat/Gameplay",
			"MIMUWSurfers/Variant_Combat/Interfaces",
			"MIMUWSurfers/Variant_Combat/UI",
			"MIMUWSurfers/Variant_SideScrolling",
			"MIMUWSurfers/Variant_SideScrolling/AI",
			"MIMUWSurfers/Variant_SideScrolling/Gameplay",
			"MIMUWSurfers/Variant_SideScrolling/Interfaces",
			"MIMUWSurfers/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
