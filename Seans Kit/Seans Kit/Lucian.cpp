#include "SeansKit.h"
#include "Lucian.h"
#include "Utilities.h"

using namespace Lucian;


PLUGIN_EVENT(void) OnGameUpdate()
{
	if (!GUtility->IsLeagueWindowFocused() || GGame->IsChatOpen())
		return;

	if (Player()->HasBuff("LucianR"))
		GOrbwalking->SetAttacksAllowed(false);
	
	else
		GOrbwalking->SetAttacksAllowed(true);


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


void Lucian::Combo()
{

	if (ComboW->Enabled())
	{
		if (W->IsReady())
		{
			IUnit* WTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, W->Range());

			if (WTarget && WTarget->IsValidTarget())
			{
				if (Player()->GetSpellRemainingCooldown(kSlotQ) / Player()->GetSpellTotalCooldown(kSlotQ) > .96)
					return;

				if (Player()->IsDashing())
					return;

				if (Player()->Distance(WTarget) > Q->Range())
				{
					auto Health = WTarget->GetHealth();
					if (Health < GDamage->GetSpellDamage(Player(), WTarget, kSlotQ) + GDamage->GetAutoAttackDamage(Player(), WTarget, true) && Health > GDamage->GetSpellDamage(Player(), WTarget, kSlotW) + GDamage->GetAutoAttackDamage(Player(), WTarget, true))
						return;
				}

				if (!Passive() || WTarget->GetHealth() < GDamage->GetSpellDamage(Player(), WTarget, kSlotW) + GDamage->GetAutoAttackDamage(Player(), WTarget, true))
					W->CastOnTarget(WTarget, kHitChanceLow);
			}
		}
	}

	if (Q->IsReady())
	{
		if (ComboQ->Enabled())
		{
			IUnit* QTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());

			if (QTarget && QTarget->IsValidTarget())
			{
				if (Player()->Distance(QTarget) < Player()->GetRealAutoAttackRange(QTarget))
				{
					if (GOrbwalking->CanAttack() || Player()->IsWindingUp())
					{
						return;
					}
				}

				if (!Passive() || QTarget->GetHealth() < GDamage->GetSpellDamage(Player(), QTarget, kSlotQ) + GDamage->GetAutoAttackDamage(Player(), QTarget, true))
				{
					Q->CastOnTarget(QTarget, kHitChanceLow);
					return;
				}
			}
		}

		if (ComboQExt->Enabled())
		{
			if (!Passive())
			{
				if (ExtendedQ())
					return;
			}
		}
	}
}

void Lucian::Harass()
{
	if (Player()->ManaPercent() < HarassMinMana->GetFloat())
		return;

	if (HarassW->Enabled())
	{
		if (W->IsReady())
		{
			IUnit* WTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, W->Range());

			if (WTarget && WTarget->IsValidTarget())
			{
				if (Player()->GetSpellRemainingCooldown(kSlotQ) / Player()->GetSpellTotalCooldown(kSlotQ) > .96)
					return;

				if (Player()->IsDashing())
					return;

				if (Player()->Distance(WTarget) > Q->Range())
				{
					auto Health = WTarget->GetHealth();
					if (Health < GDamage->GetSpellDamage(Player(), WTarget, kSlotQ) + GDamage->GetAutoAttackDamage(Player(), WTarget, true) && Health > GDamage->GetSpellDamage(Player(), WTarget, kSlotW) + GDamage->GetAutoAttackDamage(Player(), WTarget, true))
						return;
				}

				if (!Passive() || WTarget->GetHealth() < GDamage->GetSpellDamage(Player(), WTarget, kSlotW) + GDamage->GetAutoAttackDamage(Player(), WTarget, true))
					W->CastOnTarget(WTarget, kHitChanceLow);
			}
		}
	}

	if (Q->IsReady())
	{
		if (HarassQ->Enabled())
		{
			IUnit* QTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());

			if (QTarget && QTarget->IsValidTarget())
			{
				if (Player()->Distance(QTarget) < Player()->GetRealAutoAttackRange(QTarget))
				{
					if (GOrbwalking->CanAttack() || Player()->IsWindingUp())
					{
						return;
					}
				}

				if (!Passive() || QTarget->GetHealth() < GDamage->GetSpellDamage(Player(), QTarget, kSlotQ) + GDamage->GetAutoAttackDamage(Player(), QTarget, true))
				{
					Q->CastOnTarget(QTarget, kHitChanceLow);
					return;
				}
			}
		}

		if (HarassQExt->Enabled())
		{
			if (!Passive())
			{
				if (ExtendedQ())
					return;
			}
		}
	}

}

void Lucian::Laneclear()
{
	if (Player()->ManaPercent() < LaneclearMinMana->GetFloat())
		return;

	if (LaneclearQChamps && Player()->ManaPercent() > 20)
	{
		if (!Q->IsReady())
			return;

		auto Target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());

		if (Target && Target->IsValidTarget())
		{
			Q->CastOnTarget(Target);
			return;
		}

		if (ExtendedQ())
			return;
	}
}

void Lucian::AutoKillsteal()
{
	if (W->IsReady())
	{
		auto WTarget = GTargetSelector->FindTarget(QuickestKill, SpellDamage, W->Range());

		if (WTarget && WTarget->IsValidTarget())
		{
			if ((WTarget->GetHealth() < GDamage->GetSpellDamage(Player(), WTarget, kSlotW)) ||
				(GOrbwalking->GetOrbwalkingMode() != kModeNone && WTarget->GetHealth() < GDamage->GetSpellDamage(Player(), WTarget, kSlotW) + GDamage->GetAutoAttackDamage(Player(), WTarget, true)))
			{
				W->CastOnTarget(WTarget);

				if (WTarget->IsValidTarget())
					GOrbwalking->SetOverrideTarget(WTarget);

				return;
			}
		}
	}


	if (Q->IsReady())
	{
		auto QTarget = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
		auto Q2Target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q2->Range());

		if (QTarget && QTarget->IsValidTarget())
		{
			if ((QTarget->GetHealth() < GDamage->GetSpellDamage(Player(), QTarget, kSlotQ)) ||
				(GOrbwalking->GetOrbwalkingMode() != kModeNone && QTarget->GetHealth() < GDamage->GetSpellDamage(Player(), QTarget, kSlotQ) + GDamage->GetAutoAttackDamage(Player(), QTarget, true)))
			{
				Q->CastOnTarget(QTarget);

				if (QTarget->IsValidTarget())
				{
					GGame->IssueOrder(QTarget, kAttackUnit, QTarget->ServerPosition());
				}
				return;
			}
		}

		if (Q2Target && Q2Target != QTarget && Q2Target->IsValidTarget())
		{
			if ((Q2Target->GetHealth() < GDamage->GetSpellDamage(Player(), Q2Target, kSlotQ)) ||
				(GOrbwalking->GetOrbwalkingMode() != kModeNone && Q2Target->GetHealth() < GDamage->GetSpellDamage(Player(), Q2Target, kSlotQ) + GDamage->GetAutoAttackDamage(Player(), Q2Target, true)))
			{
				if (ExtendedQ())
				{
					if (Q2Target->IsValidTarget())
						GOrbwalking->SetOverrideTarget(Q2Target);

					return;
				}
			}
		}
	}

	if (Q->IsReady() && W->IsReady())
	{
		auto Target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q2->Range());
		if (Target && Target->IsValidTarget())
		{
			if (Target->GetHealth() < GDamage->GetSpellDamage(Player(), Target, kSlotQ) + GDamage->GetSpellDamage(Player(), Target, kSlotW))
			{
				if (Player()->Distance(Target) > Q->Range())
				{
					if (ExtendedQ())
					{
						W->CastOnTarget(Target);
						return;
					}
				}
				if (W->CastOnTarget(Target))
				{
					Q->CastOnTarget(Target);
					return;
				}
			}
		}
	}
}

bool Lucian::ExtendedQ()
{
	auto Target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q2->Range());

	if (!Target || !Target->IsValidTarget())
		return false;

	if (Player()->Distance(Target) < Q->Range() - Target->BoundingRadius())
		return false;

	AdvPredictionOutput output;
	Q2->RunPrediction(Target, false, kCollidesWithNothing, &output);

	if (output.HitChance >= kHitChanceLow)
	{
		std::vector<eGameObjectClassId> list = { kAIHeroClient , kobj_AI_Minion };

		auto HeroesMinions = GEntityList->GetAllUnitsOfTypes(list);

		for (auto obj : HeroesMinions)
		{
			if (obj == Player() || obj->GetTeam() == Player()->GetTeam())
				continue;

			if (!obj || !obj->IsValidTarget() || Player()->Distance(obj) > Q->Range())
				continue;

			if (IsPointInsideRectangle(Player()->ServerPosition(), Player()->ServerPosition().Extend(obj->ServerPosition(), 1100), Q2->Radius() + Target->BoundingRadius(), Target->ServerPosition()))
			{
				Q2->CastOnTarget(obj);
				return true;
			}
		}
	}

	return false;
}


PLUGIN_EVENT(void) AfterAttack(IUnit* Source, IUnit* Target)
{
	if (!GUtility->IsLeagueWindowFocused() || GGame->IsChatOpen())
		return;

	if (E->IsReady())
	{
		auto Mode = GOrbwalking->GetOrbwalkingMode();

		if (Mode == kModeCombo)
		{
			if (ComboE->Enabled())
			{
				if (Player()->Distance(GGame->CursorPosition()) > ComboERange->GetFloat())
				{
					E->CastOnPosition(GGame->CursorPosition());
					return;
				}
			}
		}

		else if (Mode == kModeMixed)
		{
			if (HarassE->Enabled())
			{
				if (Player()->Distance(GGame->CursorPosition()) > HarassERange->GetFloat())
				{
					E->CastOnPosition(GGame->CursorPosition());
					return;
				}
			}
		}
	}
}

PLUGIN_EVENT(void) OnGapCloser(GapCloserSpell const& Args)
{
	if (!GUtility->IsLeagueWindowFocused() || GGame->IsChatOpen())
		return;

	if (!MiscAntiGC->Enabled())
		return;

	if (E->IsReady())
	{
		if (Args.Source->GetTeam() == Player()->GetTeam())
			return;

		if (Player()->Distance(Args.EndPosition) <= 100)
			E->CastOnPosition(GGame->CursorPosition());
	}
}

PLUGIN_EVENT(void) OnRender()
{
	if (!GUtility->IsLeagueWindowFocused() || GGame->IsChatOpen())
		return;

	if (Q->IsReady())
	{
		if (DrawQ->Enabled())
			GRender->DrawCircle(Player()->GetPosition(), Q->Range(), Vec4(255, 255, 255, 255), 5);

		if (DrawQExt->Enabled())
			GRender->DrawCircle(Player()->GetPosition(), Q2->Range(), Vec4(255, 255, 255, 50), 5);
	}

}

bool Lucian::Passive()
{
	return Player()->HasBuff("LucianPassiveBuff");
}

void Lucian::InitEvents()
{
	GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->AddEventHandler(kEventOrbwalkAfterAttack, AfterAttack);
	GEventManager->AddEventHandler(kEventOnGapCloser, OnGapCloser);
	GEventManager->AddEventHandler(kEventOnRender, OnRender);
}

void Lucian::InitSpells()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kTargetCast, false, true, kCollidesWithNothing);
	Q->SetSkillshot(0.5f, 65.f, 1600.f, 675.f);

	Q2 = GPluginSDK->CreateSpell2(kSlotQ, kTargetCast, false, true, kCollidesWithNothing);
	Q2->SetSkillshot(0.5f, 65.f, 1600.f , 1100.f);

	W = GPluginSDK->CreateSpell2(kSlotW, kLineCast, true, true, kCollidesWithMinions);
	W->SetSkillshot(0.3f, 80.f, 1600.f, 1000.f);

	E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, false, false, kCollidesWithNothing);
	E->SetOverrideRange(425);

	R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, true, false, kCollidesWithMinions);
	R->SetSkillshot(0.2f, 110.f, 2500.f, 1400.f);
}

void Lucian::InitMenu()
{
	MainMenu = GPluginSDK->AddMenu("Seans Kit");
	LucianMenu = MainMenu->AddMenu("Lucian");

	ComboMenu = LucianMenu->AddMenu("Combo");
	ComboQ = ComboMenu->CheckBox("Use Q in combo", true);
	ComboQExt = ComboMenu->CheckBox("Use Extended Q in combo", true);
	ComboW = ComboMenu->CheckBox("Use W in combo", true);
	ComboE = ComboMenu->CheckBox("Use E in combo when:", true);
	ComboERange = ComboMenu->AddFloat("-- cursor distance > ", 0, 1000, 350);

	HarassMenu = LucianMenu->AddMenu("Harass");
	HarassMinMana = HarassMenu->AddFloat("Min. mana percent", 0, 100, 25);
	HarassQ = HarassMenu->CheckBox("Use Q in harass", true);
	HarassQExt = HarassMenu->CheckBox("Use Extended Q in harass", true);
	HarassW = HarassMenu->CheckBox("Use W in harass", false);
	HarassE = HarassMenu->CheckBox("Use E in harass when:", false);
	HarassERange = HarassMenu->AddFloat("-- cursor distance > ", 0, 1000, 350);

	LaneclearMenu = LucianMenu->AddMenu("Laneclear");
	LaneclearMinMana = LucianMenu->AddFloat("Min. mana percent", 0, 100, 25);
	LaneclearQ = LaneclearMenu->CheckBox("Use Q in laneclear when:", true);
	LaneclearQChamps = LaneclearMenu->CheckBox("-- can hit champion", true);
	LaneclearQMinions = LaneclearMenu->AddInteger("-- X minions hit [disabled]", 1, 7, 3);

	MiscMenu = LucianMenu->AddMenu("Misc.");
	MiscKS = MiscMenu->CheckBox("Auto KS", true);
	MiscAntiGC = MiscMenu->CheckBox("Anti-Gapcloser", true);

	DrawingMenu = LucianMenu->AddMenu("Drawing");
	DrawQ = DrawingMenu->CheckBox("Draw Q", true);
	DrawQExt = DrawingMenu->CheckBox("Draw Extended Q", true);

}


void Lucian::UnLoad()
{
	MainMenu->SaveSettings();
	MainMenu->Remove();

	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(kEventOrbwalkAfterAttack, AfterAttack);
	GEventManager->RemoveEventHandler(kEventOnGapCloser, OnGapCloser);
	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
}

void Lucian::Load()
{
	InitMenu();
	InitEvents();
	InitSpells();

	GGame->PrintChat("<font color='#FFFFFF'>[</font><font color='#F8F46D'>Seans Kit</font><font color='#FFFFFF'>]</font> - <font color='#FFFFFF'>Lucian</font> loaded");
}