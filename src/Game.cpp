// Game.cpp: implementation of the Game class.
//
//////////////////////////////////////////////////////////////////////
#define _WIN32_WINNT 0x0400 
#include "Game.h"
#include <process.h>
#include "math.h"
#include "URef.h"
#include <iostream>
#include <vector>
#include <string>
//#include <wincon.h>
//#include "Windows.h"
//#include "Winbase.h"

//using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Game::Game()
{
	cout<<endl<<"	Loading game object...";
	units.reserve(300);
	wisps.reserve(300);
	groups.reserve(30);
	selected.reserve(300);
	moveThreads.reserve(100);
	attackThreads.reserve(100);
	cout<<endl<<"	Loading thread object...";
	cout<<endl<<"	Loading unit objects...";
	cout<<endl<<"	Loading building objects...";
}

Game::~Game()
{
/*for(int n=0; n<moveThreadPointerList.size(); n++)
delete moveThreadPointerList[n];
	moveThreadPointerList.clear();*/
}

void Game::startgame()
{
/*SMALL_RECT temp;
temp.Left=0;
temp.Top=0;
temp.Right=50;
temp.Bottom=50;
//const SMALL_RECT* gtemp=&temp;
	SetConsoleWindowInfo(m_screen, TRUE, &temp);*/
	
	//towncenter.Alive(true);
	//GetBuilding("Barracks")->Alive(true);
	buildings.clear();
	Building tow("Towncenter", 4,5);
	Building bar("Barracks",6,7);
	tow.Alive(true);
	bar.Alive(true);
	buildings.push_back(bar);
	buildings.push_back(tow);
	
	
	units.clear();
	wisps.clear();
	groups.clear();
	selected.clear();
	units.push_back(rrifle);
	//units.push_back(rknight);
	units.push_back(rknight);
	units.push_back(rrifle);
	units.push_back(rrifle);units.push_back(rfoot);units.push_back(rfoot);units.push_back(rfoot);
	//MoveThread temp;
	attackThreads.clear();
	//moveThreads.push_back(temp);;
	keyinput.CreateThread();
	OnTurn();
}

void Game::OnTurn()
{
	HANDLE mytimer;
	mytimer=CreateWaitableTimer(NULL, false, "GameTurnTimer");
	LARGE_INTEGER ref;
	ref.QuadPart=-100;
	const LARGE_INTEGER* large=&ref;
	
	SetWaitableTimer(mytimer,large,100, NULL, NULL, true);
	try{
		for(;;)
		{
			
			
			if(WaitForSingleObject(mytimer, INFINITE)==WAIT_OBJECT_0)
			{
				//check for death of units, if yes, they are erased
				for(vector<Unit>::iterator unit=units.begin(); unit<units.end(); unit++)
				{
					if(!unit->IsAlive())
					{
						locate(0, print);
						cout<<unit->GetName()<<" is dead."<<endl;
						print+=1;
						for(vector<Unit*>::iterator sunit=selected.begin(); sunit<selected.end(); sunit++)	
						{
							if(*sunit==unit)
								selected.erase(sunit);
						}
						for(vector<UVector>::iterator uv=groups.begin(); uv<groups.end(); uv++)
						{
							for(vector<Unit*>::iterator gunit=uv->begin(); gunit<uv->end(); gunit++)
							{
								if(*gunit==unit)
									uv->erase(gunit);
							}
						}	
						for(vector<Unit>::iterator unit2=unit+1; unit2<units.end(); unit2++)
						{
							for(sunit=selected.begin(); sunit<selected.end(); sunit++)	
							{
								if(*sunit==unit2)
									*sunit-=1;
							}
							for(uv=groups.begin(); uv<groups.end(); uv++)
							{
								for(vector<Unit*>::iterator gunit=uv->begin(); gunit<uv->end(); gunit++)
								{
									if(*gunit==unit2)
										*gunit-=1;
								}
							}
							
						}
						units.erase(unit);
					}
				}
				
				for(int n=0; n<buildings.size(); n++)
				{
					//check if towncenter is finished if building
					if(buildings[n].Alive(false)==-1 && buildings[n].OnComplete()==0)
					{
						buildings[n].Alive(true);
						locate(0, print);
						cout<<buildings[n].GetName()<<" complete!"<<endl;
						print+=1;
						//Free wisps building the structure
						locate(0, print);
						cout<<"Freed "<<buildings[n].GetWisp().size()<<" wisps!"<<endl; 
						print+=1;
						for(int n=0; n<buildings[n].GetWisp().size(); n++)
						{
							wisps[buildings[n].GetWisp()[n]].setWisp("free");
						}
					}
					else if(buildings[n].Alive(false)==0)
					{
						locate(0,print);
						cout<<"Your "<<buildings[n].GetName()<<" has just been destroyed!"<<endl;
						//remember to wipe its queue
						buildings.erase(buildings.begin() + n);
						print+=1;
					}
				}
				
				//check if barrack's units have been constructed
				if(GetBuilding("Barracks")->Alive(false))//GetBuilding("Barracks")->Alive(false))
				{
					if(GetBuilding("Barracks")->getQueue(0).GetUnit()=="Footman")
					{
						if(GetBuilding("Barracks")->OnDone()==0)
						{	//vector<Unit*> temp;
							
							/*for(int c=0; c<selected.size(); c++)
							temp.push_back(&units[0]);*/
							//vector<Unit>::iterator it=units.begin();
							//add the unit to the list of units
							//Wait for map to finish printing to avoid memory errors
							//cout<<"aaaaaaaaaaaaaaaaaaaaaaaaa";
							//WaitForSingleObject(cmdout, INFINITE);
							WaitForSingleObject(cmdout, INFINITE);
							units.push_back(rfoot);	
							prints();
							cout<<"Unit "<<units[units.size()-1].GetName()<<" complete!"<<endl;
							ReleaseMutex(cmdout);
							/*for(z=0; z<units.size(); z++)
							{
							prints();
							cout<<units[z].GetName();
						}*/
							
							
							//ReleaseMutex(cmdout);
							//cout<<"aaaaaaaaaaaaaaaaaaaaaaaaa";
							
							//cout<<it->GetName();
							/*selected.clear();
							for(c=0; c<temp.size(); c++)
							selected.push_back(temp[c]);*/
						}
					}
					if(GetBuilding("Barracks")->getQueue(0).GetUnit()=="Rifleman")
					{
						if(GetBuilding("Barracks")->OnDone()==0)
						{
							//Wait for map to finish printing to avoid memory errors
							WaitForSingleObject(cmdout, INFINITE);
							
							//add the unit to the list of units
							units.push_back(rrifle);
							locate(0, print);
							cout<<"Unit "<<units[units.size()-1].GetName()<<" complete!"<<endl;
							print+=1;
							ReleaseMutex(cmdout);
						}
					}
				}
				if(GetBuilding("Barracks")->getQueue(0).GetUnit()=="Knight")
				{
					if(GetBuilding("Barracks")->OnDone()==0)
					{
						//Wait for map to finish printing to avoid memory errors
						WaitForSingleObject(cmdout, INFINITE);
						
						//add the unit to the list of units
						units.push_back(rknight);
						locate(0, print);
						cout<<"Unit "<<units[units.size()-1].GetName()<<" complete!"<<endl;
						print+=1;
						ReleaseMutex(cmdout);
					}
				}
				
		}
		
		map.print(gold, wood, units, buildings, GetBuilding("Barracks")->getQueueVector(), false);
	}
	CancelWaitableTimer(mytimer);
}
catch(...)
{
	prints(GetLastError());
	cout<<"onturn";
	system("pause"); 
	ExitProcess(-1);
}
}

void Game::OnBuild()
{
/*int n=0;
int freewisp=0;
bool failure=true;
DInt cost;
cost.a=0;
cost.b=0;
int assignw;
//if(freeswisp>0)
//{
locate(0,print);
WaitForSingleObject(cmdout, INFINITE);
prints(" Building Menu\n\n");
cout<<"  -[1] Build Barracks ( "<<rbacost.a<<"g, "<<rbacost.b<<"w )"<<endl;
if(GetBuilding("Barracks")->Alive(false))
{	print+=1;
cout<<"  -[2] Build Blacksmith ( "<<rblcost.a<<"g, "<<rblcost.b<<"w )"<<endl;}
if(GetBuilding("Barracks")->Alive(false))
{	print+=1;
cout<<"  -[3] Build Stables ( "<<rstcost.a<<"g, "<<rstcost.b<<"w )"<<endl;}
cout<<"  -[4] Exit"<<endl;
cout<<endl;
ReleaseMutex(cmdout);
print+=4;
int key;
key=getkeyA();

		switch(key)
		{
		case 97:
		case 49:
		prints("Are you sure to build barracks(y/n): ");
		key=getkeyA();
		if(key=='y'&&(gold-rbacost.a)>=0&&(wood-rbacost.b)>=0)
		{
		cout<<endl;
		cout<<"Assign a number of spirit-wisps to the task: ";
		cin>>assignw;
		for(n=0; n<wisps.size(); n++)
		{
		if(wisps[n].getWisp()=="free")
		freewisp+=1;
		}
		if(assignw<=freewisp && assignw>0)
		{
		n=0;
		while(assignw>0)
		{
		if(wisps[n].getWisp()=="free")
		{
		wisps[n].setWisp("build");
		GetBuilding("Barracks")->SetWisp(n);
		assignw-=1;
		}
		n+=1;
		}
		GetBuilding("Barracks")->SetTimer((int)(rbatime*pow(0.8,assignw-1)));
		GetBuilding("Barracks")->SetTime(time(NULL));
		}
		else
		cout<<endl<<"Not enough free wisps (you have "<<freewisp<<"free wisps)!";
		}
		break;
		
		  case 98:
		  case 50:
		  prints("Are you sure to build blacksmith(y/n): ");
		  key=getkeyA();
		  if(key=='y'&&(gold-rblcost.a)>=0&&(wood-rblcost.b)>=0)
		  {
		  cout<<endl;
		  cout<<"Assign a number of spirit-wisps to the task: ";
		  cin>>assignw;
		  for(n=0; n<wisps.size(); n++)
		  {
		  if(wisps[n].getWisp()=="free")
		  freewisp+=1;
		  }
		  if(assignw<=freewisp && assignw>0)
		  {
		  n=0;
		  while(assignw>0)
		  {
		  if(wisps[n].getWisp()=="free")
		  {
		  wisps[n].setWisp("build");
		  blacksmith.SetWisp(n);
		  assignw-=1;
		  }
		  n+=1;
		  }
		  blacksmith.SetTimer((int)(rbatime*pow(0.8,assignw)));
		  blacksmith.SetTime(time(NULL));
		  }
		  else
		  cout<<endl<<"Not enough free wisps!";
		  }
		  break;
		  
			case 99:
			case 51:
			prints("Are you sure to build stables(y/n): ");
			key=getkeyA();
			if(key=='y'&&(gold-rstcost.a)>=0&&(wood-rstcost.b)>=0)
			{
			cout<<endl;
			cout<<"Assign a number of spirit-wisps to the task: ";
			cin>>assignw;
			for(n=0; n<wisps.size(); n++)
			{
			if(wisps[n].getWisp()=="free")
			freewisp+=1;
			}
			if(assignw<=freewisp && assignw>0)
			{
			n=0;
			while(assignw>0)
			{
			if(wisps[n].getWisp()=="free")
			{
			wisps[n].setWisp("build");
			stables.SetWisp(n);
			assignw-=1;
			}
			n+=1;
			}
			stables.SetTimer((int)(rbatime*pow(0.8,assignw)));
			stables.SetTime(time(NULL));
			}
			else
			prints("\nNot enough free wisps!");
			}
			break;
			
			  case 100:
			  case 52:
			  //cost.a=0;
			  //cost.b=0
			  //pop=-1000;
			  break;
			  
				default:
				cout<<"Invalid selection!";
				break;
		}*/

for(vector<Unit*>::iterator sunit=selected.begin(); sunit<selected.end(); sunit++)	
{
	//prints(ToString((*sunit)->IsAlive())+" "+(*sunit)->GetName());
	prints(ToString((int)*sunit) + ToString((*sunit)->IsAlive())+" "+(*sunit)->GetName());
}

}

void Game::OnTrain()
{
	bool failure=true;
	DInt cost;
	int p;
	int key=0;	
	if(GetBuilding("Barracks")->Alive(false)==1)
	{
		locate(0, print);
		WaitForSingleObject(cmdout, INFINITE);
		cout<<" Training Menu"<<endl<<endl;
		cout<<"  -[1] Train Footman ( 2p, "<<rfcost.a<<"g, "<<rfcost.b<<"w )"<<endl;
		if(blacksmith.Alive(false))
			cout<<"  -[2] Train Rifleman ( 3p, "<<rrcost.a<<"g, "<<rrcost.b<<"w )"<<endl;
		if(stables.Alive(false))
			cout<<"  -[3] Train Knight ( 4p, "<<rkcost.a<<"g, "<<rkcost.b<<"w )"<<endl;
		cout<<"  -[4] Exit"<<endl;
		cout<<endl;
		ReleaseMutex(cmdout);
		print+=7;
		
		int key;
		key=getkeyA();
		switch(key)
		{
		case 97:
		case 49:
			cost=rfcost;
			p=2;
			prints("Are you sure to train footman(y/n): ");
			key=getkeyA();
			if(key==89 && gold-rfcost.a>=0 && wood-rfcost.b>=0)// && (pop.a+2)<=pop.b && GetBuilding("Barracks")->getQUnits()<5)
			{
				GetBuilding("Barracks")->queueunit('f');
				gold-=rfcost.a;
				wood-=rfcost.b;
				//pop.a+=2;
				failure=false;
				
			}
			break;
			
		case 98:
		case 50:
			cost=rrcost;
			p=3;
			prints("Are you sure to train rifleman(y/n): ");
			key=getkeyA();
			if(key==89 && blacksmith.Alive(false)&&(gold-rrcost.a)>=0&&(wood-rrcost.b)>=0)// && (pop.a+3)<=pop.b && GetBuilding("Barracks")->getQUnits()<5)
			{
				GetBuilding("Barracks")->queueunit('r');
				gold-=rrcost.a;
				wood-=rrcost.b;
				//pop.a+=3;
				failure=false;
			}
			break;
			
		case 99:
		case 51:
			cost=rkcost;
			p=4;
			prints("Are you sure to train knight(y/n): ");
			key=getkeyA();
			if(key==89 && stables.Alive(false) &&(gold-rkcost.a)>=0&&(wood-rkcost.b)>=0)// && (pop.a+4)<=pop.b && GetBuilding("Barracks")->getQUnits()<5)
			{
				GetBuilding("Barracks")->queueunit('k');
				gold-=rkcost.a;
				wood-=rkcost.b;
				//pop.a+=4;
				failure=false;
			}
			break;
			
		case 100:
		case 52:
			cost.a=0;
			cost.b=0;
			//pop=-1000;
			break;
			
		default:
			prints("Invalid selection!");
			break;
		}
				}
	
	//system("cls");
	//map.print(gold,wood,units,GetBuilding("Barracks")->getQueueVector());
	if(!GetBuilding("Barracks")->Alive(false)||(key==2&&!blacksmith.Alive(false))||(key==3&&!stables.Alive(false)))
		cout<<"You do not have the required structure!"<<endl;
	if(gold-cost.a<0 && failure)
		cout<<"Not enough gold!"<<endl;
	if(wood-cost.b<0 && failure)
		cout<<"Not enought wood!"<<endl;
	//if(pop.a+p>pop.b && failure)
	//cout<<"Not enough souls!"<<endl;
	//cout<<"end";
	
	
}

void Game::OnAttack(COORD movePos)
{try{
	if(selected.size())
	{
		DInt point;
		point.a=movePos.X;
		point.b=movePos.Y;
		
		if(GetUnit(point).size())
		{
			Unit* uTarget=*(GetUnit(point).begin());
			
			if(point.a==-777 && point.b==-777)
			{
				prints("Select unit to attack (Press Enter): ");				
				int key=0;
				while(key!=13)
				{
					key=getkeyA();
					switch(key)
					{
					case 13:  //enter
						point.a=cursor.a;
						point.b=cursor.b;
						break;
					case 37:  //Left arrow, numpad 4
					case 100:
						movecursor(-1, 0); //x, y
						break;
					case 38:  //Up arrow, numpad 8
					case 104:
						movecursor(0, -1);
						break;
					case 39:  //Right arrow, numpad 6
					case 102:
						movecursor(1, 0);
						break;
					case 40:  //Down arrow, numpad 2
					case 98:
						movecursor(0, 1);
						break;
					case 97:  //Down-Left, numpad 1
						movecursor(-1, 1);  //x, y
						break;
					case 99:  //Down-Right, numpad 3
						movecursor(1, 1);
						break;
					case 105:  // Up-Right, numpad 9
						movecursor(1, -1);
						break;
					case 103:  //Up-Left, numpad 7
						movecursor(-1, -1);
						break;
					default:
						prints("Invalid selection!");
						return;
						break;
					}
				}
			}
			WaitForSingleObject(threadCreationMutex, INFINITE);
			/*for(int n=0; n<attackThreadPointerList.size(); n++)
			{
			cout<<attackThreadPointerList.size();
			//attackThreadPointerList[n]->KillThread();
			delete attackThreadPointerList[n];
			}
			attackThreadPointerList.clear();*/
			for(int n=0; n<attackThreads.size(); n++)
			{
				if(attackThreads[n].IsCreated())
					attackThreads[n].KillThread();
			}
			for(n=0; n<moveThreads.size(); n++)
			{
				if(moveThreads[n].IsCreated())
					moveThreads[n].KillThread();
			}
			/*for(int c=0; c<moveThreads.size(); c++)
			moveThreads[c].KillThread();/
			//ttackThreads.clear();
			
			  //grouping selected units by attack range
			  /*vector<UVector> groupByRange;
			  vector<Unit*> tempUV;
			  
				tempUV.push_back(selected[0]);
				groupByRange.push_back(tempUV);
				for(n=1; n<selected.size(); n++)
				{
				for(int b=0; b<groupByRange.size(); b++)
				{
				if(selected[n]->GetRange()==(groupByRange[b])[0]->GetRange())
				{
				groupByRange[b].push_back(selected[n]);
				break;
				}
				else if(b==groupByRange.size()-1)
				{
				tempUV.clear();
				tempUV.push_back(selected[n]);
				groupByRange.push_back(tempUV);
				break;
				}
				}
				}
				
				  //creates one move thread for each different group of ranges
				  for(n=0; n<groupByRange.size(); n++)
				  {
				  DInt tempPoint=point;
				  /*AttackThread* atemp;
			attackThreadPointerList.insert(attackThreadPointerList.begin(), atemp);*/
			
			/*if(tempPoint.a-(groupByRange[n])[0]->GetPos().a>0 && tempPoint.b-(groupByRange[n])[0]->GetPos().b>=0)
			{
			tempPoint.a-=(sqrt(pow(selected[n]->GetRange(), 2)/2));
			tempPoint.b-=(sqrt(pow(selected[n]->GetRange(), 2)/2));
			}
			else if(tempPoint.a-(groupByRange[n])[0]->GetPos().a<=0 && tempPoint.b-(groupByRange[n])[0]->GetPos().b<0)
			{
			tempPoint.a+=(sqrt(pow(selected[n]->GetRange(), 2)/2));
			tempPoint.b+=(sqrt(pow(selected[n]->GetRange(), 2)/2));
			}
			else if(tempPoint.a-(groupByRange[n])[0]->GetPos().a>0 && tempPoint.b-(groupByRange[n])[0]->GetPos().b<=0)
			{
			tempPoint.a-=(sqrt(pow(selected[n]->GetRange(), 2)/2));
			tempPoint.b+=(sqrt(pow(selected[n]->GetRange(), 2)/2));
			}
			else if(tempPoint.a-(groupByRange[n])[0]->GetPos().a<=0 && tempPoint.b-(groupByRange[n])[0]->GetPos().b>0)
			{
			tempPoint.a+=(sqrt(pow(selected[n]->GetRange(), 2)/2));
			tempPoint.b-=(sqrt(pow(selected[n]->GetRange(), 2)/2));
				}*/
			
			AttackThread* tempAP = new AttackThread(selected, point, uTarget);
			attackThreads.push_back(*tempAP);
			delete tempAP;
			(attackThreads.end()-1)->CreateThread();
			
			//attackThreadPointerList[0] = new AttackThread();
			//attackThreadPointerList[0]->CreateThread();
			//AttackThread* tempAP = new AttackThread(groupByRange[n], tempPoint, GetUnit(point));
			//apt=*tempAP;
			//apt.CreateThread();
			//delete tempAP;
			//}
			/*else if(GetBuilding(point)!=NULL)
			{
			attackThreadPointerList[0] = new AttackThread(groupByRange[n], tempPoint, GetBuilding(point));
			attackThreadPointerList[0]->CreateThread();
				}*/
			//}
			
			
			/*for(n=0; n<attackThreadPointerList.size(); n++)
			delete attackThreadPointerList[n];
			attackThreadPointerList.clear();
			AttackThread* temp;
			attackThreadPointerList.insert(attackThreadPointerList.begin(), temp);
			if(GetUnit(point)!=NULL)
			{
			attackThreadPointerList[0] = new AttackThread(selected, GetUnit(point));
			attackThreadPointerList[0]->CreateThread();
			}
			else if(GetBuilding(point)!=NULL)
			{
			attackThreadPointerList[0] = new AttackThread(selected, GetBuilding(point));
			attackThreadPointerList[0]->CreateThread();
			}*/
			ReleaseMutex(threadCreationMutex);
			}
			else
				prints("Targeted unit does not exist");
		}
		else
			prints("No units selected");
		
		
	}
	catch(...)
	{
		cout<<"onatk";
		system("pause"); 
		//	ExitProcess(-1);
	}
}

void Game::OnMove(COORD movePos)
{try{
	if(selected.size()>0)
	{
		DInt point;
		point.a=movePos.X;
		point.b=movePos.Y;
		if(point.a==-777 && point.b==-777)
		{
			prints("Select point to move selected units to (Press Enter): ");				
			int key=0;
			while(key!=13)
			{
				key=getkeyA();
				switch(key)
				{
				case 13:  //enter
					point.a=cursor.a;
					point.b=cursor.b;
					break;
				case 37:  //Left arrow, numpad 4
				case 100:
					movecursor(-1, 0); //x, y
					break;
				case 38:  //Up arrow, numpad 8
				case 104:
					movecursor(0, -1);
					break;
				case 39:  //Right arrow, numpad 6
				case 102:
					movecursor(1, 0);
					break;
				case 40:  //Down arrow, numpad 2
				case 98:
					movecursor(0, 1);
					break;
				case 97:  //Down-Left, numpad 1
					movecursor(-1, 1);  //x, y
					break;
				case 99:  //Down-Right, numpad 3
					movecursor(1, 1);
					break;
				case 105:  // Up-Right, numpad 9
					movecursor(1, -1);
					break;
				case 103:  //Up-Left, numpad 7
					movecursor(-1, -1);
					break;
				default:
					prints("Invalid selection!");
					return;
					break;
				}
			}
		}
		WaitForSingleObject(threadCreationMutex, INFINITE);
		//for(int n=0; n<moveThreadPointerList.size(); n++)
		//{
		//moveThreadPointerList[n]->KillThread();
		//delete *(moveThreadPointerList.begin()+n);
		//}
		//delete mpt;
		//if(mpt)
		//{
		//delete mpt;
		/*if(moveThreads.size())
		moveThreads.erase(moveThreads.begin());
		
		  //}
		  /*cout<<moveThreadPointerList.size();
		  if(moveThreadPointerList.size())
		  {
		  delete moveThreadPointerList[0];
		  moveThreadPointerList.clear();
	}*/
		//moveThreadPointerList.clear();
		//MoveThread* temp;
		//moveThreadPointerList.push_back(temp);//insert(moveThreadPointerList.begin(), temp);
		//moveThreadPointerList[moveThreadPointerList.size()-1] = new MoveThread(selected, point);
		//moveThreadPointerList[moveThreadPointerList.size()-1]->CreateThread();
		
		/*if(mcount>=10)
		{	mcount=0;
		*(moveThreads.begin() + mcount++)=*tempMP;
		}
		else
		moveThreads.insert(moveThreads.begin() + mcount++,*tempMP);*/
		for(int c=0; c<moveThreads.size(); c++)
		{
			if(moveThreads[c].IsCreated())
				moveThreads[c].KillThread();
		}
		for(c=0; c<attackThreads.size(); c++)
		{
			if(attackThreads[c].IsCreated())
				attackThreads[c].KillThread();
		}
		/*for(int n=0; n<attackThreads.size(); n++)
		attackThreads[n].KillThread();*/
		
		MoveThread* tempMP = new MoveThread(selected, point);
		moveThreads.push_back(*tempMP);
		delete tempMP;
		//(moveThreads.begin() + mcount -1)->CreateThread();
		(moveThreads.end()-1)->CreateThread();
		ReleaseMutex(threadCreationMutex);
	}
	else
		prints("No units selected");
	}
	catch(...)
	{
		cout<<"onmove";
		system("pause"); 
		ExitProcess(-1);
	}
	
}

void Game::OnGroup(COORD topLeft, COORD botRight)
{
	if(groups.size()>=9)
		prints("Group limit reached: 9 groups max");
	else
	{
		DInt corner1, corner2;
		UVector glist;
		corner1.a=topLeft.X;
		corner1.b=topLeft.Y;
		corner2.a=botRight.X;
		corner2.b=botRight.Y;
		
		if(corner1.a==-777 && corner1.b==-777)
		{				
			//inform user of what to do
			prints("Select upper left corner of selection box (Press Enter)");
			int key=0;
			while(key!=13)
			{
				key=getkeyA();
				switch(key)
				{
				case 13:  //enter
					corner1.a=cursor.a;
					corner1.b=cursor.b;
					break;
				case 37:  //Left arrow, numpad 4
				case 100:
					movecursor(-1, 0); //x, y
					break;
				case 38:  //Up arrow, numpad 8
				case 104:
					movecursor(0, -1);
					break;
				case 39:  //Right arrow, numpad 6
				case 102:
					movecursor(1, 0);
					break;
				case 40:  //Down arrow, numpad 2
				case 98:
					movecursor(0, 1);
					break;
				case 97:  //Down-Left, numpad 1
					movecursor(-1, 1);  //x, y
					break;
				case 99:  //Down-Right, numpad 3
					movecursor(1, 1);
					break;
				case 105:  // Up-Right, numpad 9
					movecursor(1, -1);
					break;
				case 103:  //Up-Left, numpad 7
					movecursor(-1, -1);
					break;
				default:
					prints("Invalid selection!");
					return;
					break;
				}
			}
			
			prints("Select bottom right corner of selection box (Press Enter)");
			
			key=0;
			while(key!=13)
			{
				key=getkeyA();
				switch(key)
				{
				case 13:  //enter
					corner2.a=cursor.a;
					corner2.b=cursor.b;
					break;
				case 37:  //Left arrow, numpad 4
				case 100:
					movecursor(-1, 0); //x, y
					break;
				case 38:  //Up arrow, numpad 8
				case 104:
					movecursor(0, -1);
					break;
				case 39:  //Right arrow, numpad 6
				case 102:
					movecursor(1, 0);
					break;
				case 40:  //Down arrow, numpad 2
				case 98:
					movecursor(0, 1);
					break;
				case 97:  //Down-Left, numpad 1
					movecursor(-1, 1);  //x, y
					break;
				case 99:  //Down-Right, numpad 3
					movecursor(1, 1);
					break;
				case 105:  // Up-Right, numpad 9
					movecursor(1, -1);
					break;
				case 103:  //Up-Left, numpad 7
					movecursor(-1, -1);
					break;
				default:
					prints("Invalid selection!");
					return;
					break;
				}
			}
		}
		//loop through all units and find those that are within box
		for(int c=0; c<units.size(); c++)
		{
			if(units[c].GetPos().a<=corner2.a && units[c].GetPos().a>=corner1.a &&
				units[c].GetPos().b<=corner2.b && units[c].GetPos().b>=corner1.b)
			{
				//add to list of unit pointers
				glist.push_back(&units[c]);
			}
		}
		prints();
		if(glist.size())
		{
			string stemp= "Found " + ToString(glist.size()) + " units";
			prints(stemp);
			for(c=0; c<glist.size(); c++)
			{	
				WaitForSingleObject(cmdout, INFINITE);
				stemp= "  " + glist[c]->GetName() + " (" + ToString(glist[c]->GetHp().a) + "/" + ToString(glist[c]->GetHp().b) + " HP)";
				prints(stemp);
				ReleaseMutex(cmdout);
			}
			groups.push_back(glist);
			/*prints();
			cout<<"Grouped as Group "<<groups.size();*/
			stemp="Grouped as Group " + ToString(groups.size());
			prints(stemp);
			OnSelectGroup(groups.size());
		}
		else
			prints("No units found  ");
	}
}
/*
void Game::OnGroup(COORD topLeft, COORD botRight)
{
if(groups.size()>=9)
prints("Group limit reached: 9 groups max");
else
{
UVector glist;
//loop through all units and find those that are within box
for(int c=0; c<units.size(); c++)
{
if(units[c].GetPos().a<=botRight.X && units[c].GetPos().a>=topLeft.X &&
units[c].GetPos().b<=botRight.Y && units[c].GetPos().b>=topLeft.Y)
{
//add to list of unit pointers
glist.push_back(&units[c]);
}
}
prints();
cout<<" Found "<<glist.size()<<" units";
for(c=0; c<glist.size(); c++)
{	prints();
cout<<"  "<<glist[c]->GetName()<<" ("<<glist[c]->GetHp().a<<"/"<<glist[c]->GetHp().b<<" HP)";
}
groups.push_back(glist);
prints();
cout<<"Grouped as Group "<<groups.size();
OnSelectGroup(groups.size());
}
}
*/
void Game::OnSelect(DInt curpos) //captured cursor position at time of function call
{
	selected.clear();
	
	//look through building list and see if any match cursor position
	for(int n=0; n<buildings.size(); n++)
	{
		if(buildings[n].GetPos().a==curpos.a && buildings[n].GetPos().b==curpos.b)
		{
			prints();
			cout<<buildings[n].GetName()<<" has "<<buildings[n].GetHP().a<<"/"<<buildings[n].GetHP().b<<" health.";
		}
	}
	
	//look through units list and see if any match cursor position
	for(n=0; n<units.size(); n++)
	{
		if(units[n].GetPos().a==curpos.a && units[n].GetPos().b==curpos.b)
		{
			selected.push_back(&units[n]);
			locate(0, print);
			cout<<units[n].GetName()<<" ("<<units[n].GetHp().a<<"/"<<units[n].GetHp().b<<" HP) selected.";
			print+=1;
		}
	}
}

void Game::OnSelectGroup(int groupnumber)
{
	selected.clear();
	if(groupnumber<=groups.size())
	{
		selected=groups[groupnumber-1];
		prints();
		cout<<"Group "<<groupnumber<<" selected.";
	}
	else
		prints("Group not found.");
}

/*begin mini-functions*/

void Game::movecursor(int x, int y)  //x, y in reverse, with origin at top left corner, thus + equal -
{
	if(cursor.a+x<50 && cursor.a+x>=0 && cursor.b+y<50 && cursor.b+y>=0)
	{
		WaitForSingleObject(cmdout, INFINITE);
		SetConsoleTextAttribute (m_screen, map.getcell(cursor.a, cursor.b)->color);
		cout<<getmap()->getcell(cursor.a, cursor.b)->charc;
		cursor.a+=x;
		cursor.b+=y;
		ReleaseMutex(cmdout);
		WaitForSingleObject(cmdout, INFINITE);
		/*SetConsoleTextAttribute(m_screen, 13);
		cout<<"#";
		SetConsoleTextAttribute(m_screen, 7);*/
		COORD x;
		x.X=cursor.a;
		x.Y=cursor.b;
		LPDWORD dw=NULL;
		CONST WORD w=208 | map.getcell(cursor.a, cursor.b)->color;
		WriteConsoleOutputAttribute(m_screen, &w,1,x,dw);
		ReleaseMutex(cmdout);
		locate(cursor.a, cursor.b);
	}
}

void Game::MoveMouseCursor(int x, int y)
{
	try{
		if(x<50 && x>=0 && y<50 && y>=0)
		{
			WaitForSingleObject(cmdout, INFINITE);
			SetConsoleTextAttribute (m_screen, map.getcell(cursor.a, cursor.b)->color);
			cout<<getmap()->getcell(cursor.a, cursor.b)->charc;
			cursor.a=x;
			cursor.b=y;
			ReleaseMutex(cmdout);
			//locate(cursor.a, cursor.b);
			WaitForSingleObject(cmdout, INFINITE);
			/*SetConsoleTextAttribute(m_screen, 13);
			cout<<"#";
			*/
			COORD c;
			c.X=cursor.a;
			c.Y=cursor.b;
			LPDWORD dw=NULL;
			CONST WORD w=208 | map.getcell(cursor.a, cursor.b)->color;
			/*
			SetConsoleTextAttribute(m_screen, 7);*/
			WriteConsoleOutputAttribute(m_screen, &w,1,c,dw);
			
			ClearHoverArea();
			
			locate(54, 40);
			SetConsoleTextAttribute(m_screen, 15);
			switch(map.getcell(x,y)->charc[0])
			{
			case '`':
				cout<<"Open ground";
				break;
			case '^':
				cout<<"Trees";
				break;
			case '~':
			case '.':
				cout<<"Water";
				break;
			case '*':
				cout<<"Barricades";
				break;
			default:
				if(GetUnit(cursor).size())
				{	
					int row=41;
					vector<Unit*> tempVect=GetUnit(cursor);
					for(vector<Unit*>::iterator upTemp=tempVect.begin(); upTemp<tempVect.end(); upTemp++)	
					{
						locate(54, row++);
						cout<<(*upTemp)->GetName();
						locate(54, row++);
						cout<<"HP: "<<(*upTemp)->GetHp().a<<"/"<<(*upTemp)->GetHp().b;
					}

					/*locate(54, 41);
					cout<<
					locate(54, 42);
					cout<<"AC: "<<upTemp->GetArmor();
					locate(54, 43);
					cout<<"ATK: 1d20+"<<upTemp->GetAttack();
					locate(54, 44);
					cout<<"DAM: "<<upTemp->GetDamage().a<<"-"<<upTemp->GetDamage().b;*/
				}
				else if(GetBuilding(cursor)!=NULL)
				{
					Building* buTemp=GetBuilding(cursor);
					cout<<buTemp->GetName();
					locate(54, 41);
					cout<<"HP: "<<buTemp->GetHP().a<<"/"<<buTemp->GetHP().b;
				}
				break;
			}
			SetConsoleTextAttribute(m_screen, 7);
			ReleaseMutex(cmdout);
			locate(cursor.a, cursor.b);
		}
	}
	catch(...)
	{
		cout<<"bad stuff";
		ExitProcess(-1);
	}
}



void Game::locate(int x, int y)
{
	//WaitForSingleObject(cmdout, INFINITE);
	COORD dwCursorPosition;
	cout.flush();
	dwCursorPosition.X=x;
	dwCursorPosition.Y=y;
	SetConsoleCursorPosition(m_screen, dwCursorPosition);
	//	ReleaseMutex(cmdout);
}

Building* Game::GetBuilding(string nm)
{
	for(int n=0; n<buildings.size(); n++)
	{
		if(buildings[n].GetName()==nm)
			return &buildings[n];
	}
	
	return 0;
}

Building* Game::GetBuilding(DInt pos)
{
	for(int n=0; n<buildings.size(); n++)
	{
		if(buildings[n].GetPos().a==pos.a && buildings[n].GetPos().b==pos.b)
			return &buildings[n];
	}
	return NULL;
}

vector<Unit*> Game::GetUnit(DInt pos)
{
	vector<Unit*> vuTemp;
	for(int n=0; n<units.size(); n++)
	{
		if(units[n].GetPos().a==pos.a && units[n].GetPos().b==pos.b)
			vuTemp.push_back(&units[n]);
	}
	return vuTemp;
	
}

/*inline void Game::prints(string a)
{
locate(0, print);
WaitForSingleObject(cmdout, INFINITE);
cout<<a;
ReleaseMutex(cmdout);	
print+=1;
}*/

inline void Game::prints()
{
	locate(0, print);
	print+=1;
}

string Game::ToString(int a)
{
	char buf[32];
	itoa(a, buf, 10);
	return (string) buf;
}

void Game::printmap()
{
	map.print(gold, wood, units, buildings, GetBuilding("Barracks")->getQueueVector(), true);
}

void Game::moveunit(Unit *munit, DInt mpos)
{
	map.moveunit(munit, mpos);
}

inline Map* Game::getmap()
{
	return &map;
}

void Game::ClearHoverArea()
{
	for(int i=40; i<48; i++)
	{
		locate(54, i);
		cout<<"            ";
	}
}

int Game::getkeyO()
{	
	DWORD Read;
	INPUT_RECORD Event;
	
	ReadConsoleInput (m_Keyboard, &Event, 1, &Read);
	FlushConsoleInputBuffer(m_Keyboard);
	if (Event.EventType == KEY_EVENT)
	{
		if (Event.Event.KeyEvent.bKeyDown)
		{
			return Event.Event.KeyEvent.wVirtualKeyCode;
		}
	}
	
	return 0;
}

int Game::getkeyA()
{	
	DWORD Read;
	INPUT_RECORD Event;
	
	for(;;){
		
		if(WaitForSingleObject(m_Keyboard, INFINITE)==WAIT_OBJECT_0)
		{
			ReadConsoleInput (m_Keyboard, &Event, 1, &Read);
			FlushConsoleInputBuffer(m_Keyboard);
			
			if (Event.EventType == KEY_EVENT)
			{
				if (Event.Event.KeyEvent.bKeyDown)
				{
					return Event.Event.KeyEvent.wVirtualKeyCode;
				}
			}
		}
	}
}

HANDLE Game::m_Keyboard = GetStdHandle(STD_INPUT_HANDLE);
HANDLE Game::m_screen = GetStdHandle (STD_OUTPUT_HANDLE);
HANDLE Game::cmdout = CreateMutex(NULL,FALSE,"CMD_SCREEN");
HANDLE Game::threadCreationMutex = CreateMutex(NULL, FALSE, "THREAD_CREATION");
int Game::print = 50;
int Game::gold=300;
int Game::wood=100;
int Game::mcount=0;
DInt Game::cursor={0,0};
//MoveThread Game::mpt;
//AttackThread Game::apt;

Building temp;

vector<Building> bref(0); 
vector<Building> Game::buildings=bref;

Building Game::towncenter=temp;
//Building Game::barracks=temp;
Building Game::blacksmith=temp;
Building Game::stables=temp;

vector<Unit> ref(0);
vector<UVector> ref1(0);
vector<Unit*> ref2(0);
vector<MoveThread> ref3(0);
vector<AttackThread> ref4(0);

vector<Unit> Game::units=ref;
vector<Unit> Game::wisps=ref;
vector<UVector> Game::groups=ref1;
vector<Unit*> Game::selected=ref2;
/*vector<MoveThread*> Game::moveThreadPointerList=ref3;
vector<AttackThread*> Game::attackThreadPointerList=ref4;*/
vector<MoveThread> Game::moveThreads=ref3;
vector<AttackThread> Game::attackThreads=ref4;

Map t;
Map Game::map=t;


