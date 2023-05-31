// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class SimpleAlibabaCloudVoiceLibrary : ModuleRules
{
	public string ProjectDirectory
	{
		get
		{
			return Path.GetFullPath(
				  Path.Combine(ModuleDirectory, "../../../../../")
			);
		}
	}

	private void CopyAlibabaCloudVoiceDllAndLibToBinaries(string Filepath, ReadOnlyTargetRules Target)
    {
		string BinariesDirectory = Path.Combine(
			bTreatAsEngineModule ? EngineDirectory : ProjectDirectory, "Binaries",Target.Platform.ToString());

		string Filename = Path.GetFileName(Filepath);

		if (!Directory.Exists(BinariesDirectory))
		{
			Directory.CreateDirectory(BinariesDirectory);
		}

		if (!File.Exists(Path.Combine(BinariesDirectory, Filename)))
		{
			File.Copy(Filepath, Path.Combine(BinariesDirectory, Filename), true);
		}

		RuntimeDependencies.Add(Path.Combine(BinariesDirectory, Filename));
	}

	public SimpleAlibabaCloudVoiceLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string EnvironmentVersion = "Release";

			string ThirdPartyPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../SimpleAlibabaCloudVoiceLibrary/"));
			string Third_partyPath = Path.Combine(ThirdPartyPath, "NlsSdk3.X_win64", "include");
			PublicIncludePaths.Add(Third_partyPath);

			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "NlsSdk3.X_win64", "lib", "14.0", "x64", EnvironmentVersion, "libcrypto.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "NlsSdk3.X_win64", "lib", "14.0", "x64", EnvironmentVersion, "libcurld.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "NlsSdk3.X_win64", "lib", "14.0", "x64", EnvironmentVersion, "libeay32.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "NlsSdk3.X_win64", "lib", "14.0", "x64", EnvironmentVersion, "libssl-1_1-x64.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "NlsSdk3.X_win64", "lib", "14.0", "x64", EnvironmentVersion, "nlsCppSdk.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "NlsSdk3.X_win64", "lib", "14.0", "x64", EnvironmentVersion, "pthreadVC2.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "NlsSdk3.X_win64", "lib", "14.0", "x64", EnvironmentVersion, "ssleay32.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("libcrypto-1_1-x64.dll");
			PublicDelayLoadDLLs.Add("libcurld.dll");
			PublicDelayLoadDLLs.Add("libeay32.dll");
			PublicDelayLoadDLLs.Add("libssl-1_1-x64.dll");
			PublicDelayLoadDLLs.Add("nlsCppSdk.dll");
			PublicDelayLoadDLLs.Add("pthreadVC2.dll");
			PublicDelayLoadDLLs.Add("ssleay32.dll");

            string PlatformFile = Path.Combine(ThirdPartyPath,"NlsSdk3.X_win64", "lib","14.0","x64", EnvironmentVersion);

			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "libcrypto-1_1-x64.dll"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "ssleay32.dll"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "pthreadVC2.dll"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "nlsCppSdk.dll"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "libssl-1_1-x64.dll"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "libcurld.dll"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "alibabacloud-idst-common.dll"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "alibabacloud-idst-speech.dll"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "jsoncpp-0.y.z.dll"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "libcurl.dll"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "libeay32.dll"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "nlsCppOpu.dll"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "pthreadGC2.dll"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "libcurld.dll"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "libcurld.dll"), Target);

			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "libcrypto.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "libcurld.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "libeay32.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "libssl-1_1-x64.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "nlsCppSdk.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "pthreadVC2.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "ssleay32.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "alibabacloud-idst-common.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "alibabacloud-idst-speech.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "event.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "event_core.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "event_extra.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "jsoncpp.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "jsoncpp-0.y.z.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "libevent.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "libevent_core.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "libevent_extras.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "libssl.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "log4cpp.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "nlsCppOpu.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "nlsCppSdk.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "opus.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "event.lib"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "event.lib"), Target);

			//可以调试
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "ssleay32.pdb"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "nlsCppSdk.pdb"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "libeay32.pdb"), Target);
			CopyAlibabaCloudVoiceDllAndLibToBinaries(Path.Combine(PlatformFile, "jsoncpp-0.y.z.pdb"), Target);
		}
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libExampleLibrary.dylib"));
            RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/SimpleAlibabaCloudVoiceLibrary/Mac/Release/libExampleLibrary.dylib");
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			string ExampleSoPath = Path.Combine("$(PluginDir)", "Binaries", "ThirdParty", "SimpleAlibabaCloudVoiceLibrary", "Linux", "x86_64-unknown-linux-gnu", "libExampleLibrary.so");
			PublicAdditionalLibraries.Add(ExampleSoPath);
			PublicDelayLoadDLLs.Add(ExampleSoPath);
			RuntimeDependencies.Add(ExampleSoPath);
		}
	}
}
