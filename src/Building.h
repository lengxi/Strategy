// Building.h: interface for the Building class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(BUILDING)
#define BUILDING

#ifndef DINT
#include "DInt.h"
#endif
#ifndef QUEUE	
#include "Queue.h"
#endif
#include "time.h"
#include <vector>

class Building  
{
public:
	void SetTimer(int set);
	int getQUnits();

	Queue getQueue(int a);
	vector<Queue> getQueueVector();

	int queueunit(char unitc);

	int Alive(bool status);

	int OnDone();
	int OnComplete();

	DInt GetPos();

	void SubHP(int n);
	DInt GetHP();

	Building();
	Building(string name, int x, int y);
	virtual ~Building();

	void SetTime(int set);
	int GetTime();
	void SubTimer(int set);
	int GetTimer();
	vector<int> GetWisp();
	void SetWisp(int set);
	Unit getRUnit();
	string GetName();

private:
	DInt hp;
	DInt bpos;
	int uqueue;
	int live;
	vector<Queue> queue;
	Unit rqunit;

	int timer;
	int ltime;
	vector<int> wisp;
	string name;
};

#endif 
