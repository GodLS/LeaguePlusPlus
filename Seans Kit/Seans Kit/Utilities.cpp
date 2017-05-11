#include "Utilities.h"

/* Credits to AMBER */

Vec2 Vec2D(Vec2 p1, Vec2 p2)
{
	Vec2 temp;
	temp.x = (p2.x - p1.x);
	temp.y = -1 * (p2.y - p1.y);
	return temp;
}

bool IsPointInsideRectangle(Vec2 A, Vec2 B, Vec2 C, Vec2 D, Vec2 m)
{
	Vec2 AB = Vec2D(A, B);

	float C1 = -1 * (AB.y*A.x + AB.x*A.y);
	float  D1 = (AB.y*m.x + AB.x*m.y) + C1;

	Vec2 AD = Vec2D(A, D);

	float C2 = -1 * (AD.y*A.x + AD.x*A.y);
	float D2 = (AD.y*m.x + AD.x*m.y) + C2;

	Vec2 BC = Vec2D(B, C);

	float C3 = -1 * (BC.y*B.x + BC.x*B.y);
	float D3 = (BC.y*m.x + BC.x*m.y) + C3;

	Vec2 CD = Vec2D(C, D);

	float C4 = -1 * (CD.y*C.x + CD.x*C.y);
	float D4 = (CD.y*m.x + CD.x*m.y) + C4;

	return 0 >= D1 && 0 >= D4 && 0 <= D2 && 0 >= D3;
}

bool IsPointInsideRectangle(Vec3 From, Vec3 To, float Radius, Vec3 Point)
{
	float radius = Radius / 2;
	Vec2 Point2D = Point.To2D();
	Vec3 A = From + (From - To).VectorNormalize().Perpendicular() * (-radius);
	Vec2 A2D = A.To2D();
	Vec3 B = From + (From - To).VectorNormalize().Perpendicular() * (radius);
	Vec2 B2D = B.To2D();
	Vec3 C = To + (To - From).VectorNormalize().Perpendicular() * (-radius);
	Vec2 C2D = C.To2D();
	Vec3 D = To + (To - From).VectorNormalize().Perpendicular() * (radius);
	Vec2 D2D = D.To2D();

	return IsPointInsideRectangle(A2D, B2D, C2D, D2D, Point2D);
}

/* End credits */


bool IsImmobileBuff(eBuffType type)
{
	return  type == BUFF_Stun ||
			type == BUFF_Taunt ||
			type == BUFF_Snare ||
			type == BUFF_Sleep ||
			type == BUFF_Frenzy ||
			type == BUFF_Fear ||
			type == BUFF_Charm ||
			type == BUFF_Suppression ||
			type == BUFF_Flee ||
			type == BUFF_Knockup ||
			type == BUFF_Knockback;
}

bool IsImmobile(IUnit* Unit)
{
	std::vector<void*> buffs;
	Unit->GetAllBuffsData(buffs);

	for (auto *buff : buffs)
	{
		return IsImmobileBuff(GBuffData->GetBuffType(buff));
	}
	return false;
}

float IsImmobileFor(IUnit* Target)
{
	std::vector<void *> buffs;
	Target->GetAllBuffsData(buffs);

	float time = 0.0f;
	for (auto *buff : buffs)
	{
		if (GBuffData->IsActive(buff) && GGame->Time() <= GBuffData->GetEndTime(buff) && IsImmobileBuff(GBuffData->GetBuffType(buff)))
		{
			float timeleft = GBuffData->GetEndTime(buff) - GGame->Time();
			if (time < timeleft)
				time = timeleft;
			
		}
	}

	return time;
}

int CountEnemiesInRange(IUnit* Target, float Range)
{
	int Count = 0;

	auto NetworkID = Target->GetNetworkId();
	auto Enemies = GEntityList->GetAllHeros(false, true);

	for (auto Enemy : Enemies)
	{
		if (Enemy && Enemy->IsValidTarget() && Enemy->GetNetworkId() != NetworkID && Enemy->Distance(Target) <= Range)
		{
			Count++;
		}
	}

	return Count;
}
