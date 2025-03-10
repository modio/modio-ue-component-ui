/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

using System;
using System.IO;
using EpicGames.Core;
using Microsoft.Extensions.Logging;
using UnrealBuildBase;
using UnrealBuildTool;

public class ModioUILocalization : ModuleRules
{
    private void InternalLog(string message)
    {
#if UE_5_3_OR_LATER
        Logger.LogInformation(message);
#else
        Log.TraceInformation(message);
#endif
    }

    public ModioUILocalization(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "Projects"
        });

        PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Public") });
        PrivateIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Private") });

        if (Target.Type == TargetType.Editor)
        {
            string PluginName = Path.GetFileName(PluginDirectory);
            string PluginContentDir = Path.Combine(ModuleDirectory, "../../Content/Localization/ModioUI");
            string FullLocalizationPath = Path.GetFullPath(PluginContentDir);

            if (!Directory.Exists(FullLocalizationPath))
            {
                System.Console.WriteLine($"[ModioUILocalization] Localization path '{FullLocalizationPath}' does not exist. Localization will not be loaded.");
                return;
            }

            // Determine the plugin's load location (Engine or Project)
            string PluginBaseDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../"));
            bool bIsEnginePlugin = PluginBaseDir.StartsWith(Unreal.EngineDirectory.FullName);

            // Construct the localization path based on the plugin's load location
            string LocalizationPath;
            if (bIsEnginePlugin)
            {
                bool bIsMarketplacePlugin = PluginBaseDir.Contains("Marketplace");
                if (bIsMarketplacePlugin)
                {
                    LocalizationPath = $"%ENGINEDIR%Plugins/Marketplace/{PluginName}/Content/Localization/ModioUI";
                }
                else
                {
                    LocalizationPath = $"%ENGINEDIR%Plugins/{PluginName}/Content/Localization/ModioUI";
                }
            }
            else
            {
                LocalizationPath = $"%GAMEDIR%Plugins/{PluginName}/Content/Localization/ModioUI";
            }

            // Load the DefaultGame.ini file
            string GameIniPath = Path.Combine(Target.ProjectFile.Directory.FullName, "Config", "DefaultGame.ini");
            if (File.Exists(GameIniPath))
            {
                ConfigFile GameIni = new ConfigFile(new FileReference(GameIniPath), ConfigLineAction.Add);

                // Get or create the "Internationalization" section
                ConfigFileSection InternationalizationSection = GameIni.FindOrAddSection("Internationalization");

                // Check if the "LocalizationPaths" key exists
                bool bPathExists = false;
                foreach (ConfigLine Line in InternationalizationSection.Lines)
                {
                    if (Line.Key.Equals("LocalizationPaths", StringComparison.InvariantCultureIgnoreCase)
                        && Line.Value.Equals(LocalizationPath, StringComparison.InvariantCultureIgnoreCase))
                    {
                        bPathExists = true;
                        break;
                    }
                }

                // Add the path if it doesn't exist
                if (!bPathExists)
                {
                    InternationalizationSection.Lines.Add(new ConfigLine(ConfigLineAction.Add, "LocalizationPaths", LocalizationPath));
                    GameIni.Write(new FileReference(GameIniPath));
                    InternalLog($"[ModioUILocalization] Added localization path: {LocalizationPath}");
                }
            }
            else
            {
                InternalLog("Could not find DefaultGame.ini. mod.io Component UI will not be automatically added to LocalizationPaths");
            }
            
        }
    }
}