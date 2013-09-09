// Map.h: interface for the Map class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(MAP)
#define MAP

#ifndef QUEUE	
#include "Queue.h"
#endif
#include "Building.h"
#include <iostream>
//#include <vector>
//#include "windows.h"
//#include "wincon.h"

struct CELL
{
	string charc;
	int color;
};

class Map  
{
//	friend Game;
public:
	//void placeUnit(int unitnum);
	CELL* getcell(int a, int b);
	void setcolor(int color);
	void locate(int x, int y);
	void print (int g, int w, vector<Unit> unitlist, vector<Building> buildinglist, vector<Queue> queue, bool cls);
	void moveunit(Unit* munit, DInt mpos);
	Map();
	virtual ~Map();

private:
	//char bgrid[50][50];
	//Unit* ugrid[50][50];
	CELL grid[50][50];
	CELL map[50][50];
	HANDLE m_screen;
	enum color{BLACK, DBLUE, DGREEN, DSBLUE, DRED, 
				DPINK, DYELLOW, LGRAY, GRAY, BLUE, GREEN, LBLUE, RED, PINK, YELLOW, WHITE};
	CELL tempgrid[50][50];
	DInt tempResource;
	enum bgcolor
	{
		BGBLUE=16,
		BGGREEN=32,
		BGLBLUE=48,
		BGRED=64,
		BGPINK=80,
		BGYELLOW=96,
		BGWHITE=112,
		BGLGRAY=128,
	};
	COORD bufSize, bufTopleft;
	SMALL_RECT readRect;
	//CHAR_INFO ciBuffer;
};



#endif 
