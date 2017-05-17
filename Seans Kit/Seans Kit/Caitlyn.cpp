#include "SeansKit.h"
#include "Caitlyn.h"
#include "Utilities.h"
#include <string>

using namespace Caitlyn;

PLUGIN_EVENT(void) OnGameUpdate()
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

	auto Targets = GEntityList->GetAllHeros(false, true);
	for (auto Target : Targets)
	{
		if (!Target || !Target->IsValidTarget())
			continue;

		if (MiscKS->Enabled())
			AutoKillsteal(Target);

		if (MiscVsCC->Enabled() && W->IsReady())
			AutoWCC(Target);
	}


	if ((GetAsyncKeyState(MiscManualE->GetInteger()) & 0x8000) != 0 && E->IsReady())
		ManualE();
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
						if (GDamage->GetAutoAttackDamage(Player(), QTarget, true) > Health)
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

void Caitlyn::WLogic()
{
	if (!W->IsReady() || !ComboW->Enabled())
		return;

	auto Targets = GEntityList->GetAllHeros(false, true);

	for (auto Target : Targets)
	{
		if (!Target || !Target->IsValidTarget())
			continue;

		if (!ComboWWhitelistMenu->GetOption(Target->ChampionName())->Enabled())
			continue;

		if (ComboWCC->Enabled() && IsImmobileFor(Target) >= W->GetDelay() + 1.1)
			W->CastOnPosition(Target->ServerPosition());

		if (ComboWSelfPeel->Enabled())
			if (Target->Distance(Player()) <= 450)
			{
				AdvPredictionOutput* Output = nullptr;
				W->RunPrediction(Target, false, kCollidesWithNothing, Output);

				for (auto Trap : Traps)
				{
					if (Trap && Trap->IsValidObject() && Trap->Distance(Output->CastPosition) <= 200)
						return;
				}
				W->CastOnPosition(Output->CastPosition);
			}
	}
}

void Caitlyn::ELogic()
{
	if (!E->IsReady() || !ComboE->Enabled())
		return;

	if (Player()->Distance(GGame->CursorPosition()) >= ComboERange->GetFloat())
	{
		auto Position = Player()->ServerPosition() - (GGame->CursorPosition() - Player()->ServerPosition());
		E->CastOnPosition(Player()->GetPosition().Extend(Position, Player()->Distance(GGame->CursorPosition())));
		return;
	}

	auto Targets = GEntityList->GetAllHeros(false, true);

	for (auto Target : Targets)
	{
		if (!Target || !Target->IsValidTarget())
			continue;;

		if (ComboESelfPeel->Enabled() && Target->Distance(Player()) <= 300)
		{
			E->CastOnTarget(Target);
			return;
		}
	}
}

void Caitlyn::HarassQLogic()
{
	if (Q->IsReady())
	{
		if (HarassQ->Enabled())
		{
			auto QTarget = Q->FindTarget(PhysicalDamage);

			if (QTarget && QTarget->IsValidTarget())
			{
				auto Health = QTarget->GetHealth();

				if (Player()->Distance(QTarget) < Player()->GetRealAutoAttackRange(QTarget))
				{
					if (HarassQOutAARange->Enabled())
						return;

					if (GOrbwalking->CanAttack())
					{
						if (GDamage->GetAutoAttackDamage(Player(), QTarget, true) > Health)
							return;
					}
				}

				if (HarassQKS->Enabled() && GDamage->GetSpellDamage(Player(), QTarget, kSlotQ) > Health)
				{
					if (Q->CastOnTargetAoE(QTarget, 1, kHitChanceVeryHigh))
						return;
				}

				if (HarassQEnemyAlone->Enabled() && CountEnemiesInRange(QTarget, 1000) < 1)
				{
					if (Q->CastOnTarget(QTarget, kHitChanceVeryHigh))
						return;
				}

				if (HarassQImmobile->Enabled())
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

				if (Q->CastOnTargetAoE(QTarget, HarassQMinChamps->GetInteger(), kHitChanceVeryHigh))
					return;

			}
		}
	}
}

void Caitlyn::HarassWLogic()
{
	if (!W->IsReady() || !HarassW->Enabled())
		return;

	auto Targets = GEntityList->GetAllHeros(false, true);

	for (auto Target : Targets)
	{
		if (!Target || !Target->IsValidTarget())
			continue;

		if (!HarassWWhitelistMenu->GetOption(Target->ChampionName())->Enabled())
			continue;

		if (HarassWCC->Enabled() && IsImmobileFor(Target) >= W->GetDelay() + 1.1)
			W->CastOnPosition(Target->ServerPosition());

		if (HarassWSelfPeel->Enabled())
			if (Target->Distance(Player()) <= 450)
			{
				AdvPredictionOutput* Output = nullptr;
				W->RunPrediction(Target, false, kCollidesWithNothing, Output);

				for (auto Trap : Traps)
				{
					if (Trap && Trap->IsValidObject() && Trap->Distance(Output->CastPosition) <= 200)
						return;
				}
				W->CastOnPosition(Output->CastPosition);
			}
	}
}

void Caitlyn::HarassELogic()
{
	if (!E->IsReady() || !HarassE->Enabled())
		return;

	if (Player()->Distance(GGame->CursorPosition()) >= HarassERange->GetFloat())
	{
		auto Position = Player()->ServerPosition() - (GGame->CursorPosition() - Player()->ServerPosition());
		E->CastOnPosition(Player()->GetPosition().Extend(Position, Player()->Distance(GGame->CursorPosition())));
		return;
	}

	auto Targets = GEntityList->GetAllHeros(false, true);

	for (auto Target : Targets)
	{
		if (!Target || !Target->IsValidTarget())
			continue;;

		if (HarassESelfPeel->Enabled() && Target->Distance(Player()) <= 300)
		{
			E->CastOnTarget(Target);
			return;
		}
	}
}

void Caitlyn::Combo()
{
	ELogic();
	WLogic();
	QLogic();
}

void Caitlyn::Harass()
{
	HarassELogic();
	HarassWLogic();
	HarassQLogic();
}

void Caitlyn::Laneclear()
{
	if (!Q->IsReady())
		return;

	if (Player()->ManaPercent() < LaneclearMinMana->GetFloat())
		return;

	if (LaneclearQChamps->Enabled())
	{
		auto Target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());
		if (Target && Target->IsValidTarget())
		{
			if (Q->CastOnTargetAoE(Target, 1, kHitChanceVeryHigh))
				return;
		}
	}

	//if (LaneclearQMinions->Enabled())
	//{
		//Vec3 CastPosition;
		//int EnemiesHit = 0;

		//GPrediction->FindBestCastPosition(Q->Range(), Q->Radius(), true, true, true, CastPosition, EnemiesHit);

		/*if (Q->AttackMinions(LaneclearQMinions->GetInteger()))
			return;*/

		/*if (EnemiesHit >= LaneclearQMinions->GetInteger())
			Q->CastOnPosition(CastPosition);*/
	//}
}

void Caitlyn::AutoKillsteal(IUnit* Target)
{
	if (Q->IsReady() && Target->GetHealth() < GDamage->GetSpellDamage(Player(), Target, kSlotQ))
	{
		Q->CastOnTargetAoE(Target, 1, kHitChanceVeryHigh);
		return;
	}

	if (CountEnemiesInRange(Player(), 600) < 1 && CountEnemiesInRange(Target, 600) < 1 )
	{
		if (E->IsReady() && Target->GetHealth() < GDamage->GetSpellDamage(Player(), Target, kSlotE))
		{
			E->CastOnTarget(Target, kHitChanceVeryHigh);
			return;
		}

		if (R->IsReady() && Target->GetHealth() < GDamage->GetSpellDamage(Player(), Target, kSlotR))
		{
			for (auto Enemy : GEntityList->GetAllHeros(false, true))
			{
				if (!Enemy || !Enemy->IsValidTarget() || !Enemy->GetNetworkId() == Target->GetNetworkId())
					continue;

				if (IsPointInsideRectangle(Player()->GetPosition(), Target->GetPosition(), 600, Enemy->GetPosition()))
					continue;

				R->CastOnTarget(Target);
				return;
			}
		}
	}

}

void Caitlyn::AutoWCC(IUnit* Target)
{
	if (IsImmobileFor(Target) >= W->GetDelay() + 1.1)
		W->CastOnPosition(Target->ServerPosition());
}

void Caitlyn::ManualE()
{
	auto Position = Player()->ServerPosition() - (GGame->CursorPosition() - Player()->ServerPosition());

	E->CastOnPosition(Player()->GetPosition().Extend(Position, Player()->Distance(GGame->CursorPosition())));
}

PLUGIN_EVENT(void) OnDoCast(CastedSpell const& Spell)
{
	if (!GUtility->IsLeagueWindowFocused() || GGame->IsChatOpen())
		return;

	if (Spell.Caster_ != Player())
		return;

	if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
	{
		auto SpellSlot = GSpellData->GetSlot(Spell.Data_);

		if (SpellSlot == kSlotE)
		{
			if (ComboWAfterE->Enabled() && W->IsReady())
			{
				auto WTarget = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, 450);

				if (!WTarget || !WTarget->IsValidTarget())				
					return;

				if (!ComboWWhitelistMenu->GetOption(WTarget->ChampionName())->Enabled())
					return;

				W->CastOnTarget(WTarget, kHitChanceVeryHigh);
			}

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
	else if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
	{
		auto SpellSlot = GSpellData->GetSlot(Spell.Data_);

		if (SpellSlot == kSlotE)
		{
			if (Player()->ManaPercent() < HarassMinMana->GetFloat())
				return;

			if (HarassWAfterE->Enabled() && W->IsReady())
			{
				auto WTarget = GTargetSelector->FindTarget(ClosestPriority, PhysicalDamage, 450);

				if (!HarassWWhitelistMenu->GetOption(WTarget->ChampionName())->Enabled())
					return;

				if (WTarget && WTarget->IsValidTarget())
					W->CastOnTarget(WTarget, kHitChanceVeryHigh);
			}

			if (HarassQAfterE->Enabled() && Q->IsReady())
			{
				auto QTarget = Q->FindTarget(PhysicalDamage);
				if (QTarget && QTarget->IsValidTarget())
				{
					if (HarassQOutAARange->Enabled())
						if (QTarget->Distance(Player()) < 650)
							return;

					if (Q->CastOnTargetAoE(QTarget, 1, kHitChanceHigh))
						return;
				}
			}
		}
	}

}

PLUGIN_EVENT(void) OnGapCloser(GapCloserSpell const& Spell)
{
	if (!GUtility->IsLeagueWindowFocused() || GGame->IsChatOpen())
		return;

	if (!Spell.Source || !Spell.Source->IsValidTarget())
		return;

	if (Spell.Source->GetTeam() == Player()->GetTeam())
		return;

	if (ComboEAntiGC->Enabled() && E->IsReady() && GOrbwalking->GetOrbwalkingMode() == kModeCombo && Player()->Distance(Spell.EndPosition) <= 100)
	{
		E->CastOnTarget(Spell.Source, kHitChanceImpossible);
		return;
	}

	if (MiscEAntiGC->Enabled() && E->IsReady() && Player()->Distance(Spell.EndPosition) <= 100)
	{
		E->CastOnTarget(Spell.Source, kHitChanceImpossible);
		return;
	}

	if (MiscWAntiGC->Enabled() && W->IsReady() && Player()->Distance(Spell.EndPosition) <= W->Range())
	{
		for (auto Trap : Traps)
		{
			if (Trap && Trap->IsValidObject() && Trap->Distance(Spell.EndPosition) <= 200)
				return;
		}
		W->CastOnPosition(Spell.EndPosition);
		return;
	}
}

PLUGIN_EVENT(void) OnRender()
{
	if (!GUtility->IsLeagueWindowFocused() || GGame->IsChatOpen())
		return;

	if (Q->IsReady())	
		if (DrawQ->Enabled())
			GRender->DrawCircle(Player()->GetPosition(), Q->Range(), Vec4(255, 255, 255, 255), 5);
	
	if (E->IsReady())
		if (DrawEAutoDash->Enabled())
			GRender->DrawCircle(Player()->GetPosition(), ComboERange->GetFloat(), Vec4(255, 255, 255, 255), 5);
}


PLUGIN_EVENT(void) OnInterruptible(InterruptibleSpell const& Spell)
{
	if (!GUtility->IsLeagueWindowFocused() || GGame->IsChatOpen())
		return;

	if (!MiscWInterrupt->Enabled())
		return;

	if (Spell.Source->GetTeam() == Player()->GetTeam())
		return;

	if (!W->IsReady())
		return;

	if (Spell.EndTime - GGame->Time() < W->GetDelay() + 1.1)
		W->CastOnPosition(Spell.Source->ServerPosition());
}

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
	if (!GUtility->IsLeagueWindowFocused() || GGame->IsChatOpen())
		return;

	if (!ComboWBush->Enabled())
		return;

	if (!Source || !Source->IsValidTarget())
		return;

	if (Source->GetTeam() == Player()->GetTeam())
		return;

	if (Source->GetClassId() != kAIHeroClient)
		return;

	if (!ComboWWhitelistMenu->GetOption(Source->ChampionName())->Enabled())
		return;

	if (Source->Distance(Player()) > W->Range())
		return;

	if (GNavMesh->IsPointGrass(Source->GetPosition()))
	{		
		for (auto Trap : Traps)
		{
			if (Trap && Trap->IsValidObject() && Source->Distance(Trap) <= 200)
				return;
		}
		W->CastOnTarget(Source);

	}
}

PLUGIN_EVENT(void) OnBuffAdd(IUnit* Source, void* BuffData)
{
	if (!GUtility->IsLeagueWindowFocused() || GGame->IsChatOpen())
		return;

	if (Source && Source == Player() && Player()->HasBuff("caitlynheadshotrangecheck"))
	{
		if (GOrbwalking->GetOrbwalkingMode() == kModeNone)
			return;

		auto CurrentTarget = GOrbwalking->GetLastTarget();
		if (CurrentTarget && CurrentTarget->IsValidTarget() && CurrentTarget->GetHealth() < GDamage->GetAutoAttackDamage(Player(), CurrentTarget, true))
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
	if (PassiveTarget && Source && Source == PassiveTarget && !PassiveTarget->HasBuff("caitlynyordletrapinternal"))
	{
		GOrbwalking->SetOverrideTarget(nullptr);
		PassiveTarget = nullptr;
	}
}

PLUGIN_EVENT(void) OnCreateObject(IUnit* Source)
{
	if (Source && Source->GetObjectName() == "Cupcake Trap" && Source->GetNetworkId() == Player()->GetNetworkId())
		Traps.emplace_back(Source);
}

PLUGIN_EVENT(void) OnDestroyObject(IUnit* Source)
{
	if (Source && Source->GetObjectName() == "Cupcake Trap" && Source->GetNetworkId() == Player()->GetNetworkId())
	{
		//for (auto Trap : Traps)
		//{
			Traps.erase(std::remove_if(Traps.begin(),
				Traps.end(),
				[&](IUnit* Trap) { return Trap == Source; }
			));
		//}
	}
}

bool Caitlyn::Passive()
{
	return Player()->HasBuff("caitlynheadshot");
}

void Caitlyn::InitEvents()
{
	GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->AddEventHandler(kEventOnGapCloser, OnGapCloser);
	GEventManager->AddEventHandler(kEventOnRender, OnRender);
	GEventManager->AddEventHandler(kEventOnLevelUp, OnLevelUp);
	GEventManager->AddEventHandler(kEventOnDoCast, OnDoCast);
	GEventManager->AddEventHandler(kEventOnExitVisible, OnExitVisible);
	GEventManager->AddEventHandler(kEventOnBuffAdd, OnBuffAdd);
	GEventManager->AddEventHandler(kEventOnBuffRemove, OnBuffRemove);
	GEventManager->AddEventHandler(kEventOnCreateObject, OnCreateObject);
	GEventManager->AddEventHandler(kEventOnDestroyObject, OnDestroyObject);
	GEventManager->AddEventHandler(kEventOnInterruptible, OnInterruptible);
}

void Caitlyn::InitSpells()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, true, true, kCollidesWithYasuoWall);
	Q->SetSkillshot(0.625f, 90.f, 2200.f, 1300.f);

	W = GPluginSDK->CreateSpell2(kSlotW, kCircleCast, false, false, kCollidesWithNothing);
	W->SetSkillshot(0.3f, 80.f, FLT_MAX, 1000.f);

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
	ComboQMenu = ComboMenu->AddMenu("Q Settings ");
	ComboQ = ComboQMenu->CheckBox("Use Q:", true);
	ComboQMinChamps = ComboQMenu->AddInteger("-- if X targets hit", 1, 5, 2);
	ComboQImmobile = ComboQMenu->CheckBox("-- vs CCd", true);
	ComboQAfterE = ComboQMenu->CheckBox("-- after E cast", true);
	ComboQEnemyAlone = ComboQMenu->CheckBox("-- if enemy alone", true);
	ComboQKS = ComboQMenu->CheckBox("-- if killable", true);
	ComboQOutAARange = ComboQMenu->CheckBox("-- only out of AA range", false);
	ComboWMenu = ComboMenu->AddMenu("W Settings  ");
	ComboW = ComboWMenu->CheckBox("Use W:", true);
	ComboWWhitelistMenu = ComboWMenu->AddMenu("Whitelist");
	for (auto Target : GEntityList->GetAllHeros(false, true))
	{
		ComboWWhitelistMenu->CheckBox(Target->ChampionName(), true);
	}
	ComboWSelfPeel = ComboWMenu->CheckBox("-- enemy ontop of me ", true);
	ComboWCC = ComboWMenu->CheckBox("-- vs CCd", true);
	ComboWBush = ComboWMenu->CheckBox("-- enemy enter bush", true);
	ComboWAfterE = ComboWMenu->CheckBox("-- after E cast", true);
	ComboEMenu = ComboMenu->AddMenu("E Settings   ");
	ComboE = ComboEMenu->CheckBox("Use E:", true);
	ComboERange = ComboEMenu->AddFloat("-- cursor distance > ", 0, 1000, 650);
	ComboEAntiGC = ComboEMenu->CheckBox("-- anti gapcloser", true);
	ComboESelfPeel = ComboEMenu->CheckBox("-- enemy ontop of me", true);

	HarassMenu = CaitlynMenu->AddMenu("Harass ");
	HarassMinMana = HarassMenu->AddFloat("Min. mana percent", 0, 100, 25);
	//HarassQMenu = HarassMenu->AddMenu("Q Settings ");
	HarassQ = HarassMenu->CheckBox("Use Q: ", true);
	HarassQMinChamps = HarassMenu->AddInteger("-- if X targets hit ", 1, 5, 2);
	HarassQImmobile = HarassMenu->CheckBox("-- vs CCd ", true);
	HarassQAfterE = HarassMenu->CheckBox("-- after E cast ", true);
	HarassQEnemyAlone = HarassMenu->CheckBox("-- if enemy alone ", true);
	HarassQKS = HarassMenu->CheckBox("-- if killable ", true);
	HarassQOutAARange = HarassMenu->CheckBox("-- only out of AA range ", false);
	//HarassWMenu = HarassMenu->AddMenu("W Settings  ");
	HarassW = HarassMenu->CheckBox("Use W: ", false);
	HarassWWhitelistMenu = HarassMenu->AddMenu("Whitelist ");
	for (auto Target : GEntityList->GetAllHeros(false, true))
	{
		HarassWWhitelistMenu->CheckBox(Target->ChampionName(), true);
	}
	HarassWSelfPeel = HarassMenu->CheckBox("-- enemy ontop of me ", true);
	HarassWCC = HarassMenu->CheckBox("-- vs CCd ", true);
	HarassWBush = HarassMenu->CheckBox("-- enemy enter bush ", true);
	HarassWAfterE = HarassMenu->CheckBox("-- after E cast ", true);
	//HarassEMenu = HarassMenu->AddMenu("E Settings   ");
	HarassE = HarassMenu->CheckBox("Use E:", false);
	HarassERange = HarassMenu->AddFloat("-- cursor distance > ", 0, 1000, 650);
	HarassEAntiGC = HarassMenu->CheckBox("-- anti gapcloser", true);
	HarassESelfPeel = HarassMenu->CheckBox("-- enemy ontop of me", true);

	LaneclearMenu = CaitlynMenu->AddMenu("Laneclear");
	LaneclearMinMana = LaneclearMenu->AddFloat("Min. mana percent", 0, 100, 25);
	LaneclearQ = LaneclearMenu->CheckBox("Use Q in laneclear when:", true);
	LaneclearQChamps = LaneclearMenu->CheckBox("-- can hit champion", true);
	LaneclearQMinions = LaneclearMenu->AddInteger("-- X minions hit **[disabled]**", 1, 7, 3);

	MiscMenu = CaitlynMenu->AddMenu("Misc.");
	MiscKS = MiscMenu->CheckBox("Auto KS", true);
	MiscVsCC = MiscMenu->CheckBox("W CCd", true);
	MiscWInterrupt = MiscMenu->CheckBox("W Interruptible", true);
	MiscWAntiGC = MiscMenu->CheckBox("Anti-Gapcloser W", true);
	MiscEAntiGC = MiscMenu->CheckBox("Anti-Gapcloser E", true);
	MiscManualE = MiscMenu->AddKey("E to mouse", 86);

	DrawingMenu = CaitlynMenu->AddMenu("Drawing");
	DrawQ = DrawingMenu->CheckBox("Draw Q", true);
	DrawEAutoDash = DrawingMenu->CheckBox("Draw E combo auto dash range", true);
}

void Caitlyn::UnLoad()
{
	MainMenu->SaveSettings();
	MainMenu->Remove();

	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(kEventOnGapCloser, OnGapCloser);
	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
	GEventManager->RemoveEventHandler(kEventOnLevelUp, OnLevelUp);
	GEventManager->RemoveEventHandler(kEventOnDoCast, OnDoCast);
	GEventManager->RemoveEventHandler(kEventOnExitVisible, OnExitVisible);
	GEventManager->RemoveEventHandler(kEventOnBuffAdd, OnBuffAdd);
	GEventManager->RemoveEventHandler(kEventOnBuffRemove, OnBuffRemove);
	GEventManager->RemoveEventHandler(kEventOnCreateObject, OnCreateObject);
	GEventManager->RemoveEventHandler(kEventOnDestroyObject, OnDestroyObject);
	GEventManager->RemoveEventHandler(kEventOnInterruptible, OnInterruptible);

}

void Caitlyn::Load()
{
	InitMenu();
	InitEvents();
	InitSpells();

	GGame->PrintChat("<font color='#FFFFFF'>[</font><font color='#F8F46D'>Seans Kit</font><font color='#FFFFFF'>]</font> - <font color='#FFFFFF'>Caitlyn</font> loaded");
}