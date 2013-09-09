// Queue.cpp: implementation of the Queue class.
//
//////////////////////////////////////////////////////////////////////

#include "Queue.h"
//#include "URef.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Queue::Queue()
{
	timer=-777;
	ltime=0;
}

Queue::~Queue()
{

}

int Queue::GetTimer()
{
	return timer;
}

void Queue::SubTimer(int set)
{
	timer=timer-set;	
}

int Queue::GetTime()
{
	return ltime;
}

void Queue::SetTime(int set)
{
	ltime=set;
}

string Queue::GetUnit()
{
	return unitname;
}

/*Unit Queue::GetUnit()
{
	return *unit;
}*/

void Queue::SetUnit(char buildtype)
{
	switch(buildtype)
	{
	case 'f':
		unitname="Footman";
		timer=5;
		ltime=time(NULL);//time(NULL);
		break;

	case 'r':
		unitname="Rifleman";
		timer=8;
		ltime=time(NULL);
		break;

	case 'k':
		unitname="Knight";
		timer=9;
		ltime=time(NULL);
		break;
	}
}
