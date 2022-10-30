// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BloodMoon : ModuleRules
{
	public BloodMoon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NetCore" });
	}
}
