// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Rider_Unreal_Test : ModuleRules
{
	public Rider_Unreal_Test(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
