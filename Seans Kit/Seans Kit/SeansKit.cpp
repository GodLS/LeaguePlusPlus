#include "SeansKit.h"
#include "Lucian.h"
#include "Caitlyn.h"
#include <string>
#include <regex>
#include "LeeSin.h"

PluginSetup("Seans Kit")

PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{
	PluginSDKSetup(PluginSDK);

	SeansKit::LoadPlugin();
}

PLUGIN_API void OnUnload()
{
	std::string ChampionName = GEntityList->Player()->ChampionName();

	if (ChampionName == "Caitlyn")
		Caitlyn::UnLoad();
	if (ChampionName == "Lucian")
		Lucian::UnLoad();
	//if (ChampionName == "LeeSin")
	//	LeeSin::UnLoad();
}

void SeansKit::LoadPlugin()
{
	std::string ChampionName = GEntityList->Player()->ChampionName();

	if (ChampionName == "Caitlyn")
	{
		Caitlyn::Load();
		SeansKit::CheckUpdates(ChampionName, 1.0);

	}
	if (ChampionName == "Lucian")
	{
		Lucian::Load();
		SeansKit::CheckUpdates(ChampionName, 2.1);
	}

	//if (ChampionName == "LeeSin")
	//{
	//	LeeSin::Load();
	//	SeansKit::CheckUpdates(1.1);
	//}
}

IUnit* Player()
{
	return GEntityList->Player();
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

void SeansKit::CheckUpdates(std::string ChampionName, int version)
{
	std::string NewestVersion;

	if (GPluginSDK->ReadFileFromURL(format("https://raw.githubusercontent.com/GodLS/LeaguePlusPlus/master/Seans Kit/Seans Kit/%sversion.txt", ChampionName), NewestVersion))
	{
		//auto Version = regex_match(newestVersion, std::regex("[0-9]+\.[0-9]+"), "");
		if (version > stoi(NewestVersion))
		{
			GGame->PrintChat("<font color='#FFFFFF'>[</font><font color='#F8F46D'>Seans Kit</font><font color='#FFFFFF'>]</font> - BETA version.");
			return;
		}

		if (version < stoi(NewestVersion))
		{
			GGame->PrintChat("<font color='#FFFFFF'>[</font><font color='#F8F46D'>Seans Kit</font><font color='#FFFFFF'>]</font> - OUTDATED! Please redownload.");
			return;
		}

		GGame->PrintChat("<font color='#FFFFFF'>[</font><font color='#F8F46D'>Seans Kit</font><font color='#FFFFFF'>]</font> - Up to date.");

	}

}
