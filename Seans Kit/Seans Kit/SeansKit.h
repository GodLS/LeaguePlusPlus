#pragma once
#include "PluginSDK.h"

class SeansKit
{
public:
	static void LoadPlugin();
	static void CheckUpdates(char* ChampionName, int version);
	static std::string format(const char* format, ...);
};

extern IUnit* Player();