#include "SeansKit.h"
#include "LeeSin.h"
#include "Utilities.h"
#include <string>

using namespace LeeSin;

static bool IsQ1()
{
	return Player()->GetSpellName(kSlotQ) == "BlindMonkQOne";
}

void LeeSin::Load()
{
	InitMenu();
	InitEvents();
	InitSpells();

	GGame->PrintChat("<font color='#FFFFFF'>[</font><font color='#F8F46D'>Seans Kit</font><font color='#FFFFFF'>]</font> - <font color='#FFFFFF'>Caitlyn</font> loaded");
}

void LeeSin::UnLoad()
{
	MainMenu->SaveSettings();
	MainMenu->Remove();

	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	//GEventManager->RemoveEventHandler(kEventOnGapCloser, OnGapCloser);
	//GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
	//GEventManager->RemoveEventHandler(kEventOnLevelUp, OnLevelUp);
	//GEventManager->RemoveEventHandler(kEventOnDoCast, OnDoCast);
	//GEventManager->RemoveEventHandler(kEventOnExitVisible, OnExitVisible);
	//GEventManager->RemoveEventHandler(kEventOnBuffAdd, OnBuffAdd);
	//GEventManager->RemoveEventHandler(kEventOnBuffRemove, OnBuffRemove);
	//GEventManager->RemoveEventHandler(kEventOnCreateObject, OnCreateObject);
	//GEventManager->RemoveEventHandler(kEventOnDestroyObject, OnDestroyObject);
	//GEventManager->RemoveEventHandler(kEventOnInterruptible, OnInterruptible);
}

void LeeSin::InitEvents()
{
	GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);

}

void LeeSin::InitSpells()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, true, false, (kCollidesWithYasuoWall, kCollidesWithHeroes, kCollidesWithMinions));
	Q->SetSkillshot(0.25f, 65.f, 1750.f, 1080.f);
	W = GPluginSDK->CreateSpell2(kSlotW, kTargetCast, false, false, kCollidesWithNothing);
	W->SetOverrideRange(700);
	E = GPluginSDK->CreateSpell2(kSlotE, kTargetCast, false, false, kCollidesWithNothing);
	E->SetOverrideRange(400);
	E2 = GPluginSDK->CreateSpell2(kSlotE, kTargetCast, false, false, kCollidesWithNothing);
	E2->SetOverrideRange(570);
	R = GPluginSDK->CreateSpell2(kSlotR, kTargetCast, false, false, kCollidesWithNothing);
	R->SetOverrideRange(375);
}

void LeeSin::InitMenu()
{
	MainMenu = GPluginSDK->AddMenu("Seans Kit");
	LeeSinMenu = MainMenu->AddMenu("Lee Sin");

	InsecKey = MainMenu->AddKey("Insec", 86);
}

void LeeSin::Insec()
{
	auto InsecTarget = GTargetSelector->GetFocusedTarget();
	if (!InsecTarget || !InsecTarget->IsValidTarget())
		return;

	//GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());

	// Q into range
	if (IsQ1() /*&&  (!W->IsReady() || W->IsReady() && Player()->Distance(InsecTarget) > W->Range())*/)
	{
		AdvPredictionOutput* Output = nullptr;
		Q->RunPrediction(InsecTarget, false, kCollidesWithHeroes | kCollidesWithMinions | kCollidesWithYasuoWall, Output);

		if (Output->HitChance >= kHitChanceVeryHigh)
			Q->CastOnPosition(Output->CastPosition);

		
	}
}

PLUGIN_EVENT(void) LeeSin::OnGameUpdate()
{
	if (!GUtility->IsLeagueWindowFocused() || GGame->IsChatOpen())
		return;

	if (Player()->IsWindingUp())
		return;

	eOrbwalkingMode Mode = GOrbwalking->GetOrbwalkingMode();

	switch (Mode)
	{
	case kModeLastHit:
		break;
	case kModeMixed:
		Harass();
		break;
	case kModeLaneClear:
		Laneclear();
		break;
	case kModeCombo:
		Combo();
		break;
	case kModeFreeze:
		break;
	case kModeCustom:
		break;
	case kModeNone:
		break;
	}

	if ((GetAsyncKeyState(InsecKey->GetInteger()) & 0x8000) != 0 && R->IsReady())
		Insec();
}

void LeeSin::Combo()
{

}

void LeeSin::Harass()
{
}

void LeeSin::Laneclear()
{
}


