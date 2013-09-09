// Unit.h: interface for the Unit class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(UNIT)
#define UNIT

#ifndef DINT
#include "DInt.h"
#endif
#include <iostream>
#include <windows.h>
#include "winbase.h"

using namespace std;

class Unit  
{
public:
	//HANDLE GetMutex();
	void SetPos(DInt newpos);
	void setWisp(string set);
	string getWisp();
	void InitUnit(Unit aunit);

	DInt GetPos();
	bool IsAlive();
	string GetDes();
	string GetName();
	int GetArmor();
	int GetSpeed();
	int GetRange();
	DInt GetDamage();
	int GetAttack();
	int GetAttackRate();
	void SubHp(int sub);
	DDouble GetHp();
	Unit(double hit, int atk, int atkrate, int damlow, int damhigh, int rng, int spd, string nm, string des, 
		int posx, int posy, int ac);
	Unit(int x, int y);
	Unit();
	virtual ~Unit();
	//int id;

int speed;
//	Unit& operator=( Unit& aunit);

private:
	DDouble hp;			//unit hp, .a=current hp, .b=max hp
	int attack;			//number to add to 1d20 attack roll to beat armor score
	int attackRate;		//represents how many 100ms timesteps between attacks
	DInt damage;		//represents lower and upper limits of attack damage
	int range;			//number of squares to target to attack
				//represents how many 100ms timesteps between movements of one square
	string name;
	string description;
	DInt upos;			//x,y postions of the unit
	int armor;			//number to add to base armor 10 to bolster defense
	string wispwork;	//task wisp is currently doing?
	//HANDLE unitMutex;

};

#endif 
