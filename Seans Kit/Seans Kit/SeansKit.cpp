#include "SeansKit.h"
#include "Lucian.h"

PluginSetup("ADC Package")

PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{
	PluginSDKSetup(PluginSDK);

	SeansKit::LoadPlugin();

}

void SeansKit::LoadPlugin()
{
	std::string ChampionName = GEntityList->Player()->ChampionName();

	if (ChampionName == "Lucian")
		Lucian::Load();
}
