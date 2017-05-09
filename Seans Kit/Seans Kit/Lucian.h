#pragma once

namespace Lucian
{
	extern void Load();

	static void InitEvents();
	static void InitSpells();
	static void InitMenu();

	static void Combo();
	static void Harass();
	static void Laneclear();

	static bool ExtendedQ();
	static void AutoKillsteal();
	static bool Passive();

	static IUnit* Player();

	static ISpell2 *Q, *Q2, *W, *E, *R;

	static IMenu* MainMenu;

	static IMenu* LucianMenu;

	static IMenu* ComboMenu;
	static IMenu* HarassMenu;
	static IMenu* LaneclearMenu;
	static IMenu* MiscMenu;
	static IMenu* DrawingMenu;

	static IMenuOption* ComboQ;
	static IMenuOption* ComboQExt;
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