#include "SeansKit.h"
#include "Caitlyn.h"
#include "Utilities.h"

using namespace Caitlyn;

IUnit* PassiveTarget;
// Eventually make proper Q KS damage like for varus Q, account for minions

PLUGIN_EVENT(void) OnGameUpdate()
{
	if (!GUtility->IsLeagueWindowFocused() || GGame->IsChatOpen())
		return;

	if (Player()->HasBuff("CaitlynR"))
		GOrbwalking->SetAttacksAllowed(false);
	else
		GOrbwalking->SetAttacksAllowed(true);

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
	default:
		break;
	}

	if (MiscKS->Enabled())
		AutoKillsteal();
}

void Caitlyn::QLogic()
{
	if (Q->IsReady())
	{
		if (ComboQ->Enabled())
		{
			auto QTarget = Q->FindTarget(PhysicalDamage);

			if (QTarget && QTarget->IsValidTarget())
			{
				auto Health = QTarget->GetHealth();

				if (Player()->Distance(QTarget) < Player()->GetRealAutoAttackRange(QTarget))
				{
					if (ComboQOutAARange->Enabled())
						return;

					if (GOrbwalking->CanAttack())
					{
						if (GDamage->GetSpellDamage(Player(), QTarget, kSlotQ) < Health || GDamage->GetAutoAttackDamage(Player(), QTarget, true) > Health)
							return;
					}
				}

				if (ComboQKS->Enabled() && GDamage->GetSpellDamage(Player(), QTarget, kSlotQ) > Health)
				{
					if (Q->CastOnTargetAoE(QTarget, 1, kHitChanceVeryHigh))
						return;
				}

				if (ComboQEnemyAlone->Enabled() && CountEnemiesInRange(QTarget, 1000) < 1)
				{
					if (Q->CastOnTarget(QTarget, kHitChanceVeryHigh))
						return;
				}

				if (ComboQImmobile->Enabled())
				{
					if (IsImmobileFor(QTarget) >=
						Player()->Distance(QTarget) / Q->Speed() + Q->GetDelay())
					{
						Q->CastOnPosition(QTarget->ServerPosition());
						return;
					}

					if (QTarget->HasBuffOfType(BUFF_Slow))
						if (Q->CastOnTargetAoE(QTarget, 1, kHitChanceVeryHigh))
							return;

					if (Q->CastOnTargetAoE(QTarget, 1, kHitChanceDashing))
						return;
				}

				if (Q->CastOnTargetAoE(QTarget, ComboQMinChamps->GetInteger(), kHitChanceVeryHigh))
					return;

			}
		}
	}
}

void Caitlyn::Combo()
{

	QLogic();

}

void Caitlyn::Harass()
{
	
}

void Caitlyn::Laneclear()
{
	/*if (LaneclearQChamps && Player()->ManaPercent() > 20)
	{
		if (!Q->IsReady())
			return;

		auto Target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());

		if (Target && Target->IsValidTarget())
		{
			Q->CastOnTarget(Target);
			return;
		}
	}*/
}

void Caitlyn::AutoKillsteal()
{

}

PLUGIN_EVENT(void) OnDoCast(CastedSpell const& Spell)
{
	if (Spell.Caster_ != Player())
		return;

	if (GOrbwalking->GetOrbwalkingMode() != kModeCombo)
		return;

	auto SpellSlot = GSpellData->GetSlot(Spell.Data_);

	if (SpellSlot == kSlotE)
	{
		if (ComboQAfterE->Enabled() && Q->IsReady() && Player()->ManaPercent() > 9)
		{
			auto QTarget = Q->FindTarget(PhysicalDamage);
			if (QTarget && QTarget->IsValidTarget())
			{
				if (ComboQOutAARange->Enabled())
					if (QTarget->Distance(Player()) < 650)
						return;

				if (Q->CastOnTargetAoE(QTarget, 1, kHitChanceHigh))
					return;
			}
		}
	}

}

PLUGIN_EVENT(void) OnGapCloser(GapCloserSpell const& Spell)
{
	
}

PLUGIN_EVENT(void) OnRender()
{

}

PLUGIN_EVENT(void) AfterAttack(IUnit* Source, IUnit* Target)
{
	
}

//PLUGIN_EVENT(void) OnSpellCast(CastedSpell const& Spell)
//{
//
//}

PLUGIN_EVENT(void) OnLevelUp(IUnit* Source, int NewLevel)
{
	if (Source && Source == Player() && NewLevel)
	{
		if (NewLevel == 11)
			R->SetOverrideRange(2500);
		else if (NewLevel == 16)
			R->SetOverrideRange(3000);
	}

}

PLUGIN_EVENT(void) OnExitVisible(IUnit* Source)
{
	
}

PLUGIN_EVENT(void) OnBuffAdd(IUnit* Source, void* BuffData)
{
	if (Source == Player() && Player()->HasBuff("caitlynheadshotrangecheck"))
	{
		if (GOrbwalking->GetOrbwalkingMode() == kModeNone)
			return;

		auto Targets = GEntityList->GetAllHeros(false, true);
		for (auto Target : Targets)
		{
			if (Target && Target->IsValidTarget() && Target->HasBuff("caitlynyordletrapinternal") && Target->Distance(Player()) <= 1300)
			{
				GOrbwalking->SetOverrideTarget(Target);
				PassiveTarget = Target;
			}
				
		}
	}
}

PLUGIN_EVENT(void) OnBuffRemove(IUnit* Source, void* BuffData)
{
	if (PassiveTarget && Source == PassiveTarget && !PassiveTarget->HasBuff("caitlynyordletrapinternal"))
	{
		GOrbwalking->SetOverrideTarget(nullptr);
		PassiveTarget = nullptr;
	}
}

bool Caitlyn::Passive()
{
	return Player()->HasBuff("caitlynheadshot");
}

void Caitlyn::InitEvents()
{
	GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->AddEventHandler(kEventOrbwalkAfterAttack, AfterAttack);
	GEventManager->AddEventHandler(kEventOnGapCloser, OnGapCloser);
	GEventManager->AddEventHandler(kEventOnRender, OnRender);
	GEventManager->AddEventHandler(kEventOnLevelUp, OnLevelUp);
	GEventManager->AddEventHandler(kEventOnDoCast, OnDoCast);
	GEventManager->AddEventHandler(kEventOnExitVisible, OnExitVisible);
	GEventManager->AddEventHandler(kEventOnBuffAdd, OnBuffAdd);
	GEventManager->AddEventHandler(kEventOnBuffRemove, OnBuffRemove);
	//GEventManager->AddEventHandler(kEventOnSpellCast, OnSpellCast);
}

void Caitlyn::InitSpells()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, true, true, kCollidesWithYasuoWall);
	Q->SetSkillshot(0.625f, 90.f, 2200.f, 1300.f);

	W = GPluginSDK->CreateSpell2(kSlotW, kCircleCast, false, true, kCollidesWithNothing);
	W->SetSkillshot(0.3f, 80.f, 1600.f, 1000.f);

	E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, true, false, kCollidesWithMinions | kCollidesWithHeroes | kCollidesWithYasuoWall);
	E->SetSkillshot(0.125f, 70.f, 1600.f, 1000.f);

	R = GPluginSDK->CreateSpell2(kSlotR, kTargetCast, true, false, kCollidesWithHeroes | kCollidesWithYasuoWall);
	R->SetOverrideRange(2000);
}

void Caitlyn::InitMenu()
{
	MainMenu = GPluginSDK->AddMenu("Seans Kit");
	CaitlynMenu = MainMenu->AddMenu("Caitlyn");

	ComboMenu = CaitlynMenu->AddMenu("Combo");
	ComboQ = ComboMenu->CheckBox("Use Q in combo:", true);
	ComboQMinChamps = ComboMenu->AddInteger("-- if X targets hit", 1, 5, 1);
	ComboQImmobile = ComboMenu->CheckBox("-- vs cc", true);
	ComboQAfterE = ComboMenu->CheckBox("-- after E cast", true);
	ComboQEnemyAlone = ComboMenu->CheckBox("-- if enemy alone", true);
	ComboQKS = ComboMenu->CheckBox("-- if killable", true);
	ComboQOutAARange = ComboMenu->CheckBox("-- only out of AA range", false);

	ComboW = ComboMenu->CheckBox("Use W in combo", true);
	ComboE = ComboMenu->CheckBox("Use E in combo when:", true);
	ComboERange = ComboMenu->AddFloat("-- cursor distance > ", 0, 1000, 350);

	HarassMenu = CaitlynMenu->AddMenu("Harass");
	HarassQ = HarassMenu->CheckBox("Use Q in harass", true);
	HarassQExt = HarassMenu->CheckBox("Use Extended Q in harass", true);
	HarassW = HarassMenu->CheckBox("Use W in harass", false);
	HarassE = HarassMenu->CheckBox("Use E in harass when:", false);
	HarassERange = HarassMenu->AddFloat("-- cursor distance > ", 0, 1000, 350);

	LaneclearMenu = CaitlynMenu->AddMenu("Laneclear");
	LaneclearQ = LaneclearMenu->CheckBox("Use Q in laneclear when:", true);
	LaneclearQChamps = LaneclearMenu->CheckBox("-- can hit champion", true);
	LaneclearQMinions = LaneclearMenu->AddInteger("-- X minions hit [disabled]", 1, 7, 3);

	MiscMenu = CaitlynMenu->AddMenu("Misc.");
	MiscKS = MiscMenu->CheckBox("Auto KS", true);
	MiscAntiGC = MiscMenu->CheckBox("Anti-Gapcloser", true);

	DrawingMenu = CaitlynMenu->AddMenu("Drawing");
	DrawQ = DrawingMenu->CheckBox("Draw Q", true);
	DrawQExt = DrawingMenu->CheckBox("Draw Extended Q", true);
}

void Caitlyn::UnLoad()
{
	MainMenu->SaveSettings();
	MainMenu->Remove();

	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(kEventOrbwalkAfterAttack, AfterAttack);
	GEventManager->RemoveEventHandler(kEventOnGapCloser, OnGapCloser);
	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
	GEventManager->RemoveEventHandler(kEventOnLevelUp, OnLevelUp);
	GEventManager->RemoveEventHandler(kEventOnDoCast, OnDoCast);
	GEventManager->RemoveEventHandler(kEventOnExitVisible, OnExitVisible);
	GEventManager->RemoveEventHandler(kEventOnBuffAdd, OnBuffAdd);
	GEventManager->RemoveEventHandler(kEventOnBuffRemove, OnBuffRemove);

}

void Caitlyn::Load()
{
	InitMenu();
	InitEvents();
	InitSpells();

	GGame->PrintChat("<font color='#FFFFFF'>[</font><font color='#F8F46D'>Seans Kit</font><font color='#FFFFFF'>]</font> - <font color='#FFFFFF'>Caitlyn</font> loaded");
}