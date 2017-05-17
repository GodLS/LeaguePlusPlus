#pragma once

namespace LeeSin
{
	extern void Load();
	extern void UnLoad();

	static void InitEvents();
	static void InitSpells();
	static void InitMenu();

	static void Combo();
	static void Harass();
	static void Laneclear();

	static void Insec();

	PLUGIN_EVENT(void) OnGameUpdate();

	static ISpell2 *Q, *W, *E, *E2, *R;

	static IMenu* MainMenu;

	static IMenu* LeeSinMenu;

	static IMenuOption* InsecKey;
}