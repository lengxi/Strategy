// Map.cpp: implementation of the Map class.
//
//////////////////////////////////////////////////////////////////////

#include "Map.h"
//#include "windows.h"
//#include "Game.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Game.h"
//#include "math.h"

using namespace std;

//B=barracks, T=towncenter, S=stables, C= blacksmith (buildings=brown), ~= water(blue/lblue), *=trees(green) 
//R=Rifleman, F=Footman, K= Knight (range from white, yellow, red)
//#=cursor (pink/purple)

Map::Map()
{
	cout<<endl<<endl<<"	Loading map object...";
	m_screen = GetStdHandle (STD_OUTPUT_HANDLE);
	bufSize.X=50;
	bufSize.Y=50;
	bufTopleft.X=0;
	bufTopleft.Y=0;
	readRect.Left=0;
	readRect.Top=0;
	readRect.Bottom=50;
	readRect.Right=50;
	/*ciBuffer.Attributes=7;
	ciBuffer.Char.AsciiChar='`';*/
	string temp[50];
	ifstream getMap;
	getMap.open("map1.txt");
	for(int n=0; n<50; n++)
		getMap>>temp[n];
	for(n=0; n<50; n++)
	{
		for(int i=0; i<50; i++)
		{
			map[i][n].charc=temp[n][i];
			switch(temp[n][i])
			{
			case '`':
				map[i][n].color=LGRAY;
				break;
			case '^':
				map[i][n].color=DGREEN;
				break;
			case '~':
				map[i][n].color=BLUE;
				break;
			case '.':
				map[i][n].color=LBLUE;
				break;
			case '*':
				map[i][n].color=DRED;
				break;

			}
		}
	}
	getMap.close();
}

Map::~Map()
{

}

void Map::print(int g, int w, vector<Unit> unitlist, vector<Building> buildinglist, vector<Queue> queue, bool cls)
{	
	WaitForSingleObject(Game::cmdout, INFINITE);
	int a,b;
	//locate(0,0);
	bool done=false;
	
	CHAR_INFO ciBuffer[50][50];
	ReadConsoleOutput(m_screen, &ciBuffer[0][0], bufSize, bufTopleft, &readRect);
	for(a=0; a<50; a++)
	{		
		for(int b=0; b<50; b++)
		{
			tempgrid[a][b].charc=ciBuffer[a][b].Char.AsciiChar;
			tempgrid[a][b].color=ciBuffer[a][b].Attributes;
			/*locate(a,b);
			setcolor(tempgrid[b][a].color);
			cout<<tempgrid[b][a].charc;*/
		}
	}

	//if screen was cleared, we will make sure map is printed
	

	//builds the current map out of unit lists and building lists
	for(a=0; a<50; a++)
	{
		for(b=0; b<50; b++)
		{
			done=false;
			for(int u=0; u<unitlist.size(); u++)
			{
				if(unitlist[u].GetPos().a==a && unitlist[u].GetPos().b==b)
				{
					done=true;
					if(unitlist[u].GetName()=="Footman")
					{
						grid[a][b].charc="F";

						if(unitlist[u].GetHp().a/unitlist[u].GetHp().b<=0.33)
							grid[a][b].color=RED;

						else if(unitlist[u].GetHp().a/unitlist[u].GetHp().b>=0.66)
							grid[a][b].color=LBLUE;

						else
							grid[a][b].color=YELLOW;
					}
					else if(unitlist[u].GetName()=="Rifleman")
					{
						grid[a][b].charc="R";

						if(unitlist[u].GetHp().a/unitlist[u].GetHp().b<=0.33)
							grid[a][b].color=RED;

						else if(unitlist[u].GetHp().a/unitlist[u].GetHp().b>=0.66)
							grid[a][b].color=LBLUE;

						else
							grid[a][b].color=YELLOW;
					}
					else if(unitlist[u].GetName()=="Knight")
					{
						grid[a][b].charc="K";

						if(unitlist[u].GetHp().a/unitlist[u].GetHp().b<=0.33)
							grid[a][b].color=RED;

						else if(unitlist[u].GetHp().a/unitlist[u].GetHp().b>=0.66)
							grid[a][b].color=LBLUE;

						else
							grid[a][b].color=YELLOW;
					}
				}
			}
			if(!done)
			{
			for(u=0; u<buildinglist.size(); u++)
			{
				if(buildinglist[u].GetPos().a==a && buildinglist[u].GetPos().b==b)
				{
					done=true;
					grid[a][b].charc=buildinglist[u].GetName()[0];

					if(buildinglist[u].GetHP().a/buildinglist[u].GetHP().b<=0.1)
						grid[a][b].color=DRED;

					else 
						grid[a][b].color=DSBLUE;
				}
			}
			}
			if(!done)
			{
				grid[a][b].charc=map[a][b].charc[0];
				grid[a][b].color=map[a][b].color;
			}
		}
	}

	//if screen isnt cleared, then reprint only changes
	if(!cls)
	{
		for(a=0; a<50; a++)
		{
			for(int b=0; b<50; b++)
			{
				//system("cls");
				//cout<<a<<b<<" ";
				if((tempgrid[b][a].charc!=grid[a][b].charc || tempgrid[b][a].color!=grid[a][b].color)&& (a!=Game::cursor.a || b!=Game::cursor.b))
				{
					locate(a,b);
					setcolor(grid[a][b].color);
					cout<<grid[a][b].charc;
				}
			}
		}
	}

	//else reprint it all
	else
	{
		for(a=0; a<50; a++)
		{
			locate(0,a);
			for(int b=0; b<50; b++)
			{
				setcolor(grid[b][a].color);
				cout<<grid[b][a].charc;	
			}
			cout<<endl;
		}
	}

	
	//copies current grid to be checked against next iteration
	/*for(a=0; a<50; a++)
	{		
		for(int b=0; b<50; b++)
		if(grid[a][b].charc[0] && grid[a][b].color)
			tempgrid[a][b]=grid[a][b];	
	}*/
	
	if(tempResource.a!=g || cls)
	{
		//prints wood/gold amt
		setcolor(LGRAY);
		locate(54,3);
		cout<<"Gold: "<<g<<endl;
	}
	if(tempResource.b!=w || cls)
	{
		setcolor(LGRAY);
		locate(54,4);
		cout<<"Wood: "<<w<<endl;
	}
	tempResource.a=g;
	tempResource.b=w;
	//locate(54,5);
	//cout<<"Population: "<<pp.a<<"/"<<pp.b;

	int y=10;

	//prints the queue of units waiting
	for(a=0; a<queue.size(); a++)
	{		
		if(queue[a].GetTimer()>=0 && a==0)
		{
			setcolor(LGRAY);
			locate(54,y);
			cout<<queue[a].GetUnit()<<": "<<queue[a].GetTimer()<<" seconds left"<<endl;
			y++;
		}
		else if(queue[a].GetTimer()>=0)
		{
			setcolor(LGRAY);
			locate(54, y);
			cout<<queue[a].GetUnit()<<": pending..."<<endl;			
			y++;
		}
	}


	locate(Game::cursor.a, Game::cursor.b);
	setcolor(LGRAY);
	ReleaseMutex(Game::cmdout);

 }

void Map::locate(int x, int y)
{
	//WaitForSingleObject(Game::cmdout, INFINITE);
	COORD dwCursorPosition;
	//cout.flush();
	dwCursorPosition.X=x;
	dwCursorPosition.Y=y;
	SetConsoleCursorPosition(m_screen, dwCursorPosition);
//	ReleaseMutex(Game::cmdout);
}



void Map::setcolor(int color)
{
	SetConsoleTextAttribute (m_screen, color);
}

CELL* Map::getcell(int a, int b)
{
	return &grid[a][b];
}

/*void Map::placeUnit(int unitnum)
{
	cout<<"Select position to place unit:";
	for(;;)
	{
}*/

void Map::moveunit(Unit *munit, DInt mpos)
{
try{
	if(munit->GetPos().a>=0 &&munit->GetPos().a<50 && munit->GetPos().b>=0 &&munit->GetPos().b<50 && mpos.a>=0 && mpos.a<50 && mpos.b>=0 && mpos.b<50)	
	{
		WaitForSingleObject(Game::cmdout, INFINITE);	
		locate(munit->GetPos().a, munit->GetPos().b);

		//setcolor(ciBuffer.Attributes);
		//cout<<ciBuffer.Char.AsciiChar;

		setcolor(map[mpos.a][mpos.b].color);
		cout<<map[mpos.a][mpos.b].charc[0];
		
		/*bufTopleft.X=0;
		bufTopleft.Y=0;
		readRect.Left=mpos.a;
		readRect.Top=mpos.b;
		readRect.Bottom=mpos.b;
		readRect.Right=mpos.a;

		ReadConsoleOutput(m_screen, &ciBuffer, bufSize, bufTopleft, &readRect);*/
		
		locate(mpos.a, mpos.b);

		if(munit->GetHp().a/munit->GetHp().b>=0.66)
		{
			setcolor(LBLUE);
			cout<<munit->GetName()[0];
			setcolor(LGRAY);
		}
		else if(munit->GetHp().a/munit->GetHp().b<=0.33)
		{
			setcolor(RED);
			cout<<munit->GetName()[0];
			setcolor(LGRAY);
		}
		else
		{
			setcolor(YELLOW);
			cout<<munit->GetName()[0];
			setcolor(LGRAY);
		}
		munit->SetPos(mpos);
		locate(Game::cursor.a, Game::cursor.b);
		
		ReleaseMutex(Game::cmdout);
		
	}
}
catch(...)
{
	cout<<"moveunit";
	ExitProcess(-1);
}
	
}
