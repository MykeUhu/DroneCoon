using System.IO;
using UnrealBuildTool;

public class DroneCoon : ModuleRules
{
	public DroneCoon(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"GameplayTags",
			"EnhancedInput",
			"PhysicsCore",
			"DedicatedServers",
			"HTTP"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			
			"Slate",
			"SlateCore",
			"UMG",
			"JsonUtilities",
			"Json"
		});
	}
}