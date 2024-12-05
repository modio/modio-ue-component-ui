using System.IO;
using UnrealBuildTool;

public class ModioUICore : ModuleRules
{
	public ModioUICore(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"UMG",
			"Slate",
			"SlateCore",
			"InputCore",
			"Modio",
			"CommonUI",
			"GameplayTags"
		});

		if (Target.bBuildEditor)
		{
			PublicDependencyModuleNames.Add("Settings");
		}

		PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public") });
		PrivateIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Private") });
	}
}