// Building.cpp: implementation of the Building class.
//
//////////////////////////////////////////////////////////////////////

#include "Building.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Building::Building(string n, int x, int y)
{
	timer=-777;
	uqueue=0;
	name=n;
	live=1;
	bpos.a=x;
	bpos.b=y;
}

Building::Building()
{
	timer=-777;
	uqueue=0;
	live=1;
}

Building::~Building()
{

}

DInt Building::GetHP()
{
	return hp;
}

void Building::SubHP(int n)
{
	if((hp.a-n)>=0)
		hp.a -= n;
	else
		hp.a=0;
}

DInt Building::GetPos()
{
	return bpos;
}

int Building::OnDone()
{
	if(queue.size())
	{
		if(queue[0].GetTimer()==-777)
			return -1;
		else if(queue[0].GetTimer()<=0)
		{
			/*rqunit=queue[0].GetUnit();
			for(int n=1; n<uqueue; n++)
				queue[n-1]=queue[n];
			queue.pop_back();*/
			queue.erase(queue.begin());
			if(queue.size())
				queue.begin()->SetTime(time(NULL));
			return 0;
		}
		else
		{
			int n=time(NULL)-queue[0].GetTime();
			queue[0].SubTimer(n);
			queue[0].SetTime(time(NULL));
			if(queue[0].GetTimer()<=0)
			{
				queue.erase(queue.begin());
				if(queue.size())
					queue.begin()->SetTime(time(NULL));
				return 0;
			}
			else
				return queue[0].GetTimer();
		}
	}
}

int Building::OnComplete()
{
	if(timer==-777)
		return -1;

	else if(timer<=0)
	{
		timer=-777;
		return 0;
	}

	else
	{
		int n=time(NULL)-ltime;
		timer=timer-n;	
		ltime=time(NULL);
		if(timer==0)
		{
			timer=-777;
			return 0;
		}
		else
			return timer;
	}
}

int Building::Alive(bool status)
{
	//makes the building alive
	if(status=true)
	{
		live=1;
		hp.a=800;
		hp.b=800;
		return 1;
	}
	//if building has no health, it is changed to dead
	if(hp.a<=0)
	{
		live=-1;
		return 0;
	}
	//building is already dead
	if(live==-1)
		return -1;
	//building is alive, yay
	if(live==1)
		return 1;
	//should not get to this point
	return -1;
}

int Building::queueunit(char unitc)
{
	Queue temp;
	switch(unitc)
	{
	case 'f':
		temp.SetUnit('f');
		queue.push_back(temp);
		uqueue+=1;
		return 1;
		break;

	case 'r':
		temp.SetUnit('r');
		queue.push_back(temp);
		uqueue+=1;
		return 1;
		break;

	case 'k':
		temp.SetUnit('k');
		queue.push_back(temp);
		uqueue+=1;
		return 1;
		break;
	}
	return 0;
}

Queue Building::getQueue(int a)
{
	Queue tq;
	if(queue.size()>a)
		return queue[a];
	return tq;
}

int Building::getQUnits()
{
	return uqueue;
}

Unit Building::getRUnit()
{
	return rqunit;
}

int Building::GetTimer()
{
	return timer;
}

void Building::SubTimer(int set)
{
	timer=timer-set;	
}

int Building::GetTime()
{
	return ltime;
}

void Building::SetTime(int set)
{
	ltime=set;
}

vector<int> Building::GetWisp()
{
	return wisp;
}

void Building::SetWisp(int set)
{
	//int n=0;
	//while(set[n])
	//{
		wisp.push_back(set);
		//n++;
	//}
}

void Building::SetTimer(int set)
{
	timer=set;
}

vector<Queue> Building::getQueueVector()
{
	return queue;
}

string Building::GetName()
{
	return name;
}