#pragma once
namespace Caitlyn
{
	extern void Load();
	extern void UnLoad();

	static void InitEvents();
	static void InitSpells();
	static void InitMenu();

	static void Combo();
	static void Harass();
	static void Laneclear();

	static void AutoKillsteal(IUnit* Target);
	static void AutoWCC(IUnit* Target);
	static bool Passive();
	static void ManualE();

	static void QLogic();
	static void WLogic();
	static void ELogic();

	static void HarassQLogic();
	static void HarassWLogic();
	static void HarassELogic();

	static ISpell2 *Q, *W, *E, *R;

	static IMenu* MainMenu;

	static IMenu* CaitlynMenu;

	static IMenu* ComboMenu;
	static IMenu* ComboQMenu;
	static IMenu* ComboWMenu;
	static IMenu* ComboWWhitelistMenu;
	static IMenu* ComboEMenu;


	static IMenu* HarassMenu;
	static IMenu* HarassQMenu;
	static IMenu* HarassWMenu;
	static IMenu* HarassWWhitelistMenu;
	static IMenu* HarassEMenu;

	static IMenu* LaneclearMenu;
	static IMenu* MiscMenu;
	static IMenu* DrawingMenu;

	static IMenuOption* ComboQ;
	static IMenuOption* ComboQImmobile;
	static IMenuOption*	ComboQMinChamps;
	static IMenuOption* ComboQOutAARange;
	static IMenuOption* ComboQAfterE;
	static IMenuOption* ComboQEnemyAlone;
	static IMenuOption* ComboQKS;
	static IMenuOption* ComboW;
	static IMenuOption* ComboWCC;
	static IMenuOption* ComboWAfterE;
	static IMenuOption* ComboWSelfPeel;
	static IMenuOption* ComboWBush;
	static IMenuOption* ComboE;
	static IMenuOption* ComboERange;
	static IMenuOption* ComboEAntiGC;
	static IMenuOption* ComboESelfPeel;
	static IMenuOption* ComboSeparator1;
	static IMenuOption* ComboSeparator2;


	static IMenuOption* HarassMinMana;
	static IMenuOption* HarassQ;
	static IMenuOption* HarassQImmobile;
	static IMenuOption*	HarassQMinChamps;
	static IMenuOption* HarassQOutAARange;
	static IMenuOption* HarassQAfterE;
	static IMenuOption* HarassQEnemyAlone;
	static IMenuOption* HarassQKS;
	static IMenuOption* HarassW;
	static IMenuOption* HarassWCC;
	static IMenuOption* HarassWAfterE;
	static IMenuOption* HarassWSelfPeel;
	static IMenuOption* HarassWBush;
	static IMenuOption* HarassE;
	static IMenuOption* HarassERange;
	static IMenuOption* HarassEAntiGC;
	static IMenuOption* HarassESelfPeel;

	static IMenuOption* LaneclearMinMana;
	static IMenuOption* LaneclearQ;
	static IMenuOption* LaneclearQMinions;
	static IMenuOption* LaneclearQChamps;

	static IMenuOption* MiscKS;
	static IMenuOption* MiscEAntiGC;
	static IMenuOption* MiscWAntiGC;
	static IMenuOption* MiscWInterrupt;
	static IMenuOption* MiscVsCC;
	static IMenuOption* MiscManualE;
	static IMenuOption* MiscAANetTrap;

	static IMenuOption* DrawQ;
	static IMenuOption* DrawEAutoDash;


	static IUnit* PassiveTarget;
	static std::vector<IUnit*> Traps;



};