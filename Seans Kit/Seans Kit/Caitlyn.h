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

	static void AutoKillsteal();
	static bool Passive();

	static void QLogic();


	static ISpell2 *Q, *Q2, *W, *E, *R;

	static IMenu* MainMenu;

	static IMenu* CaitlynMenu;

	static IMenu* ComboMenu;
	static IMenu* HarassMenu;
	static IMenu* LaneclearMenu;
	static IMenu* MiscMenu;
	static IMenu* DrawingMenu;

	static IMenuOption* ComboQ;
	static IMenuOption* ComboQImmobile;
	//static IMenuOption* ComboQReduceLevel;
	static IMenuOption*	ComboQMinChamps;
	static IMenuOption* ComboQOutAARange;
	static IMenuOption* ComboQAfterE;
	static IMenuOption* ComboQEnemyAlone;
	static IMenuOption* ComboQKS;
	static IMenuOption* ComboW;
	static IMenuOption* ComboE;
	static IMenuOption* ComboERange;

	static IMenuOption* HarassQ;
	static IMenuOption* HarassQExt;
	static IMenuOption* HarassW;
	static IMenuOption* HarassE;
	static IMenuOption* HarassERange;

	static IMenuOption* LaneclearQ;
	static IMenuOption* LaneclearQMinions;
	static IMenuOption* LaneclearQChamps;

	static IMenuOption* MiscKS;
	static IMenuOption* MiscAntiGC;

	static IMenuOption* DrawQ;
	static IMenuOption* DrawQExt;



};