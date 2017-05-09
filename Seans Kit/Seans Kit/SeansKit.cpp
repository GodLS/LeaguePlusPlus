#include "SeansKit.h"
#include "Lucian.h"
#include <string>
#include <regex>

PluginSetup("Seans Kit")

PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{
	PluginSDKSetup(PluginSDK);

	SeansKit::LoadPlugin();
	SeansKit::CheckUpdates(1);

}

void SeansKit::LoadPlugin()
{
	std::string ChampionName = GEntityList->Player()->ChampionName();

	if (ChampionName == "Lucian")
		Lucian::Load();
}

/* Thanks SoNiice for version checker. */
std::string SeansKit::format(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	auto size = _vscprintf(format, args);
	std::string result(++size, 0);
	vsnprintf_s(const_cast<char*>(result.data()), size, _TRUNCATE, format, args);
	va_end(args);

	return result;
}

void SeansKit::CheckUpdates(int version)
{
	std::string newestVersion;

	if (GPluginSDK->ReadFileFromURL(format("https://raw.githubusercontent.com/GodLS/LeaguePlusPlus/master/Seans%20Kit/Seans%20Kit/version.txt"), newestVersion))
	{
		newestVersion = regex_replace(newestVersion, std::regex("[^0-9]+"), "");

		if (version < stoi(newestVersion))
		{
			GGame->PrintChat("<font color='#FFFFFF'>[</font><font color='#F8F46D'>Seans Kit</font><font color='#FFFFFF'>]</font> - OUTDATED! Please redownload.");
			return;
		}

		GGame->PrintChat("<font color='#FFFFFF'>[</font><font color='#F8F46D'>Seans Kit</font><font color='#FFFFFF'>]</font> - Up to date.");

	}

}
