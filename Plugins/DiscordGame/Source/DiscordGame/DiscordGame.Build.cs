// Copyright (c) MykeUhu

using UnrealBuildTool;

public class DiscordGame : ModuleRules
{
	public DiscordGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseUnity = false;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"DiscordGameSDK",
			"Engine",
			"Projects",
			"DeveloperSettings"
		});
		PrivateDependencyModuleNames.AddRange(new string[] 
		{
			"Slate",
			"SlateCore"
		});
	}
}