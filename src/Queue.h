// Queue.h: interface for the Queue class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(QUEUE)
#define QUEUE

#ifndef UNIT
#include "Unit.h"
#endif

#include "time.h"

class Queue  
{
public:
	void SetUnit(char buildtype);
	string GetUnit();
	//char GetUnit(int type);
	void SetTime(int set);
	int GetTime();
	void SubTimer(int set);
	int GetTimer();
	Queue();
	virtual ~Queue();

private:
	int timer;
	int ltime;
	string unitname;

};

#endif 