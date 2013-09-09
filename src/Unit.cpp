// Unit.cpp: implementation of the Unit class.
//
//////////////////////////////////////////////////////////////////////

#include "Unit.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Unit::Unit(double hit, int atk, int atkrate, int damlow, int damhigh, int rng, int spd, string nm, string des, 
		   int posx, int posy, int ac)
{
	hp.a=hit;
	hp.b=hit;
	attack=atk;
	attackRate=atkrate;
	damage.a=damlow;
	damage.b=damhigh;
	range=rng;
	speed=spd;
	name=nm;
	description=des;
	upos.a=posx;
	upos.b=posy;
	armor=ac;
}

Unit::Unit(int x, int y)
{
	upos.a=x;
	upos.b=y;
}

Unit::Unit()
{

}

Unit::~Unit()
{

}

DDouble Unit::GetHp()
{
	return hp;
}

void Unit::SubHp(int sub)
{
	if((hp.a - sub)>=0)
		hp.a -= sub;
	else
		hp.a=0;
}

int Unit::GetAttack()
{
	return attack;
}

int Unit::GetAttackRate()
{
	return attackRate;
}

DInt Unit::GetDamage()
{
	return damage;
}

int Unit::GetRange()
{
	return range;
}

int Unit::GetSpeed()
{
	return speed;
}

string Unit::GetName()
{
	return name;
}

string Unit::GetDes()
{
	return description;
}

int Unit::GetArmor()
{
	return armor;
}

bool Unit::IsAlive()
{
	if(hp.a>0)
		return true;
	else
		return false;
}

DInt Unit::GetPos()
{
	return upos;
}



/*Unit& Unit::operator=( Unit& aunit)
{
	hp=aunit.GetHp();
	attack=aunit.GetAtk();
	damage=aunit.GetDam();
	range=aunit.GetRng();
	speed=aunit.GetSpd();
	name=aunit.GetName();
	description=aunit.GetDes();
	upos=aunit.GetPos();
	armor=aunit.GetAc();

	return *this;
}
*/

void Unit::InitUnit(Unit aunit)
{
	hp=aunit.GetHp();
	attack=aunit.GetAttack();
	damage=aunit.GetDamage();
	range=aunit.GetRange();
	speed=aunit.GetSpeed();
	name=aunit.GetName();
	description=aunit.GetDes();
	upos=aunit.GetPos();
	armor=aunit.GetArmor();
}

string Unit::getWisp()
{
	return wispwork;
}

void Unit::setWisp(string set)
{
	wispwork=set;
}

void Unit::SetPos(DInt newpos)
{
	upos.a=newpos.a;
	upos.b=newpos.b;
}

/*HANDLE Unit::GetMutex()
{
	return unitMutex;
}*/
