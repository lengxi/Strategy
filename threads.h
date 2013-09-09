#ifndef _ThreadHEADER_
#define _ThreadHEADER_

//project dependencies
#include "Unit.h"
#include "Building.h"
#include "DInt.h"
#include <vector>
#include "math.h"
//#include <algorithm>

//win32 specific
//#include <windows.h>
//#include "winbase.h"

//extern HANDLE cmdout;

//end project dependencies

struct param;

class InputThread {

public:
	//constructors
	InputThread ();

	//destructor
	virtual ~InputThread ();

	virtual bool CreateThread ();

	int	IsCreated ()
	{	return (this->hThread != NULL);	}

	DWORD		Timeout;

	HANDLE	GetThreadHandle ()
	{	return this->hThread;	}
	DWORD	GetThreadId ()
	{	return this->hThreadId;	}
	HANDLE	GetMainThreadHandle ()
	{	return this->hMainThread;	}
	DWORD	GetMainThreadId ()
	{	return this->hMainThreadId;	}

protected:

	//overrideable
	virtual unsigned long Process (void* parameter);	

	DWORD		hThreadId;
	HANDLE		hThread;
	DWORD		hMainThreadId;
	HANDLE		hMainThread;
	//param* this_param;

private:
	static int runProcess (void* Param);
	void MouseEventHandler(char type, MOUSE_EVENT_RECORD mouseEvent);
	bool leftButtonDown;
	bool rightButtonDown;
	bool drag;
	COORD oldMousePosition;
	
};

struct param {
	InputThread*	pThread;
};


class MoveThread : public InputThread  
{
public:
	MoveThread(vector<Unit*> list, DInt loc);
	MoveThread();
	virtual ~MoveThread();
	virtual bool CreateThread();
	void KillThread();

protected:
	virtual unsigned long Process (void* parameter);

public:
	//static vector<HANDLE> moveMutexList;
	vector<Unit*> movelist; //list of units to be moved
	DInt moveloc;            //location to be moved to  
	void MoveUnits(vector<DInt> increm, vector<int> xshift, vector<int> yshift, vector<DInt> temppos);
	DInt GetModifiedShift(int posSym, int smallPosA, int smallPosB, int bigPosA, int bigPosB);
};

class AttackThread : public MoveThread  
{
public:
	AttackThread(vector<Unit*> list, DInt loc, Unit* target);
	AttackThread(vector<Unit*> list, DInt loc, Building* target);
	AttackThread();
	virtual ~AttackThread();
	virtual bool CreateThread();

protected:
	virtual unsigned long Process (void* parameter);

private:
	void AttackUnits(vector<DInt> increm, vector<int> xshift, vector<int> yshift, vector<DInt> temppos);
	vector<Unit*> unitList;   //list of units that will attack
	Unit* unitTarget;         //target to be attacked
	Building* buildingTarget; //building target to be attacked
};

#endif // _InputThreadHEADER_