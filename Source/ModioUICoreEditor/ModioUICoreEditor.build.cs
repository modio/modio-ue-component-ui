using System.IO;
using UnrealBuildTool;

public class ModioUICoreEditor : ModuleRules
{
    public ModioUICoreEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", "CoreUObject", "AppFramework",
            "Engine", "InputCore", "UnrealEd", "Slate", "SlateCore", "Modio", "ModioUICore",
            "AssetTools", "DetailCustomizations", "PropertyEditor", "Blutility", "UMG", "UMGEditor", "Kismet",
            "KismetCompiler", "DeveloperToolSettings"
        });

        PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public") });
        PrivateIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Private") });
    }
}