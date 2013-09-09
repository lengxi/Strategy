#define _WIN32_WINNT 0x0400 


#include "threads.h"
//#include "winnt.h"
//#include "winuser.h"
//#include "windef.h"
//#include "math.h"
#include "Game.h"
#include <string>

#define Distance(ap, bp) sqrt(pow(bp.a-ap.a,2)+pow(bp.b-ap.b,2))
//end project dependencies

/*void locate(int x, int y)
{
	HANDLE hConsoleOutput;
	COORD dwCursorPosition;
	cout.flush();
	dwCursorPosition.X=x;
	dwCursorPosition.Y=y;
	hConsoleOutput=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}*/
void locate(int x, int y)
{
HANDLE m_screen=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwCursorPosition;

	dwCursorPosition.X=x;
	dwCursorPosition.Y=y;
	SetConsoleCursorPosition(m_screen, dwCursorPosition);
	
}
//*********************************************************
//default constructor for class InputThread
//*********************************************************
InputThread::InputThread ()
{
	this->hThread		= NULL;
	this->hThreadId		= 0;
	this->hMainThread	= ::GetCurrentThread ();
	this->hMainThreadId = ::GetCurrentThreadId ();
	this->Timeout		= 2000; //milliseconds
	leftButtonDown=false;
	rightButtonDown=false;
	drag=false;
}

//*********************************************************
//destructor for class CObject
//*********************************************************
InputThread::~InputThread ()
{
	//waiting for the thread to terminate
	if (this->hThread) {
		if (::WaitForSingleObject (this->hThread, this->Timeout) == WAIT_TIMEOUT)
			::TerminateThread (this->hThread, 1);

		::CloseHandle (this->hThread);
	}
}

//*********************************************************
//working method
//*********************************************************
unsigned long InputThread::Process (void* parameter)
{

	//a mechanism for terminating thread should be implemented
	//not allowing the method to be run from the main thread
	if (::GetCurrentThreadId () == this->hMainThreadId)
		return 0;
	else {
			HANDLE m_input;
			HANDLE m_screen;
			m_input = GetStdHandle (STD_INPUT_HANDLE);
			m_screen = GetStdHandle (STD_OUTPUT_HANDLE);

			//set console to recognize mouse input
			

			for(;;)
			{
			DWORD fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT; 
			if(!SetConsoleMode(m_input, fdwMode)) 
			{
				Game::prints("Error setting mouse input. You may not be able to use the mouse as a control device.");
				system("pause");
				//ExitProcess(0);
			}
				if(WaitForSingleObject(m_input, INFINITE)==WAIT_OBJECT_0)
				{
					DWORD numRead;
					INPUT_RECORD bufEvent[128]; 

					ReadConsoleInput (m_input, bufEvent, 128, &numRead);
					//FlushConsoleInputBuffer(m_input);

					for(int i=0; i<numRead; i++) {
					if (bufEvent[i].EventType == KEY_EVENT)
					{
						if (bufEvent[i].Event.KeyEvent.bKeyDown)
						{	
							COORD tempCoord;			
							switch(bufEvent[i].Event.KeyEvent.wVirtualKeyCode)
							{
							case 66:   //b/B
								Game::OnBuild();
								break;
							case 84:   //t/T
								Game::OnTrain();
								break;
							case 13:   //Enter
								Game::OnSelect(Game::cursor);
								break;
							case 71:  //g/G
								tempCoord.X=-777;
								tempCoord.Y=-777;
								Game::OnGroup(tempCoord, tempCoord);
								break;
							case 77:  //m/M
								tempCoord.X=-777;
								tempCoord.Y=-777;
								Game::OnMove(tempCoord);
								break;
							case 65:  //a/A
								tempCoord.X=-777;
								tempCoord.Y=-777;
								Game::OnAttack(tempCoord);

							case 37:  //Left arrow, numpad 4
							case 100:
								Game::movecursor(-1, 0); //x, y
								break;
							case 38:  //Up arrow, numpad 8
							case 104:
								Game::movecursor(0, -1);
								break;
							case 39:  //Right arrow, numpad 6
							case 102:
								Game::movecursor(1, 0);
								break;
							case 40:  //Down arrow, numpad 2
							case 98:
								Game::movecursor(0, 1);
								break;

							case 82:  //R/r
								WaitForSingleObject(Game::cmdout, INFINITE);
								system("cls");
								Game::printmap();
								Game::print=50;
								ReleaseMutex(Game::cmdout);
								break;

							case 49:	//1-9, main keyboard not numpad
							case 50:
							case 51:
							case 52:
							case 53:
							case 54:
							case 55:
							case 56:
							case 57:
								Game::OnSelectGroup(bufEvent[i].Event.KeyEvent.wVirtualKeyCode-48);
								break;

							case 97:  //Down-Left, numpad 1
								Game::movecursor(-1, 1);  //x, y
								break;
							case 99:  //Down-Right, numpad 3
								Game::movecursor(1, 1);
								break;
							case 105:  //Up-Right, numpad 9
								Game::movecursor(1, -1);
								break;
							case 103:  //Up-Left, numpad 7
								Game::movecursor(-1, -1);
								break;			
							}

						}
					}
					else if(bufEvent[i].EventType == MOUSE_EVENT)
					{/*	Game::locate(0,60);
						WaitForSingleObject(Game::cmdout, INFINITE);
						cout<<bufEvent[i].Event.MouseEvent.dwButtonState<<"  "<<leftButtonDown<<"  "<<rightButtonDown<<"   "<<drag<<"              ";
						ReleaseMutex(Game::cmdout);*/
try{
						if(bufEvent[i].Event.MouseEvent.dwEventFlags==MOUSE_MOVED)  //when the user moves the cursor using the mouse
							Game::MoveMouseCursor(bufEvent[i].Event.MouseEvent.dwMousePosition.X, bufEvent[i].Event.MouseEvent.dwMousePosition.Y);
						if(bufEvent[i].Event.MouseEvent.dwButtonState==1)  //mouseButtonDown==false && drag==false && when the user first presses a button
							leftButtonDown=true;
						else if(bufEvent[i].Event.MouseEvent.dwButtonState==2 && bufEvent[i].Event.MouseEvent.dwEventFlags!=MOUSE_MOVED)  //mouseButtonDown==false && drag==false && when the user first presses a button
							MouseEventHandler('m', bufEvent[i].Event.MouseEvent);
						if(bufEvent[i].Event.MouseEvent.dwButtonState==0)
						{
							leftButtonDown=false;
							//rightButtonDown=false;
						}
						if(leftButtonDown==true && drag==false && bufEvent[i].Event.MouseEvent.dwEventFlags==MOUSE_MOVED)  //when the user begins to drag
						{
							drag=true;
							oldMousePosition=bufEvent[i].Event.MouseEvent.dwMousePosition;
						}
						//else if(leftButtonDown==true && drag==false && bufEvent[i].Event.MouseEvent.dwEventFlags==0)  //when the user clicks on something
							//MouseEventHandler('s', bufEvent[i].Event.MouseEvent);  leftButtonDown==true &&
						//else if(rightButtonDown==true && drag==false && bufEvent[i].Event.MouseEvent.dwEventFlags==0)  //when the user clicks on something
							//MouseEventHandler('m', bufEvent[i].Event.MouseEvent);
						else if( drag==true && bufEvent[i].Event.MouseEvent.dwEventFlags==0)  //when the user finsihes the drag
							MouseEventHandler('d', bufEvent[i].Event.MouseEvent);
						}catch(...)
	{
		cout<<"death and glory for the input";
		ExitProcess(-1);
	}
					}
				}
			}
		}
		
		
		
		return 0;
	}

}

//*********************************************************
//creates the thread
//*********************************************************
bool InputThread::CreateThread ()
{

	if (!this->IsCreated ()) {
		param*	this_param = new param;
		this_param->pThread	= this;
		this->hThread = ::CreateThread (NULL, 0, (unsigned long (__stdcall *)(void *))this->runProcess, (void *)(this_param), 0, &this->hThreadId);
		return this->hThread ? true : false;
	}
	return false;

}

//*********************************************************
//creates the thread
//*********************************************************
int InputThread::runProcess (void* Param)
{
	InputThread*	thread;
	thread			= (InputThread*)((param*)Param)->pThread;
	delete	((param*)Param);

	return thread->Process (0);
}
void InputThread::MouseEventHandler(char type, MOUSE_EVENT_RECORD mouseEvent)
{
	try{
	DInt cursor;
	switch(type)
	{
	case 's':
		//leftButtonDown=!leftButtonDown;
		cursor.a=mouseEvent.dwMousePosition.X;
		cursor.b=mouseEvent.dwMousePosition.Y;
		Game::OnSelect(cursor);
		break;
	case 'm':
		cursor.a=mouseEvent.dwMousePosition.X;
		cursor.b=mouseEvent.dwMousePosition.Y;
		if(Game::GetUnit(cursor).size()==0)
		{//Game::prints("onmove");
			Game::OnMove(mouseEvent.dwMousePosition);
			
		}
		else
		{//Game::prints("onattack");
			Game::OnAttack(mouseEvent.dwMousePosition);
			
		}
		//rightButtonDown=!rightButtonDown;
		break;
	case 'd':
		drag=false;
		leftButtonDown=false;
		Game::OnGroup(oldMousePosition, mouseEvent.dwMousePosition);
		break;
	}
	}
	catch(...)
	{
		cout<<"death and glory for the mouse";
		ExitProcess(-1);
	}
}







//////////////////////////////////////////
//									     /
//MoveThread code                        /
//										 /
//////////////////////////////////////////

MoveThread::MoveThread(vector<Unit*> list, DInt loc)
{
	InputThread::InputThread();
	movelist=list;
	moveloc=loc;
}

MoveThread::MoveThread()
{
	InputThread::InputThread();
}

MoveThread::~MoveThread()
{
	if(this->hThread) 
	{
		for(int n=0; n<movelist.size(); n++)
			movelist[n]=NULL;
		::CloseHandle (this->hThread);
		::ExitThread (1);
		
		//ExitProcess(0);
	}
}

bool MoveThread::CreateThread ()
{
	InputThread::CreateThread();
	return true;
}

void MoveThread::KillThread()
{
	if(this->hThread) 
	{
		/*for(int n=0; n<movelist.size(); n++)
			movelist[n]=NULL;*/
		//delete this_param;
		::TerminateThread (this->hThread,1);
		//::SuspendThread(this->hMainThread);
		//ExitThread(1);
		::CloseHandle (this->hThread);
		
		//ExitThread(1);
		/*
		//_endthread();
		::CloseHandle (this->hThread);*/
		//ExitProcess(0);
	}
}

unsigned long MoveThread::Process (void* parameter)
{
	//a mechanism for terminating thread should be implemented
	//not allowing the method to be run from the main thread
	if (::GetCurrentThreadId () == this->hMainThreadId)
		return 0;
	else 
	{
		int octSymbPos=1;

		vector<int> xshift, yshift;
		int total=movelist.size();
		vector<DInt> temppos;
		vector<DInt> increm;
		DInt g;
		g.a=0;
		g.b=0;
		//vector<HANDLE> vhTemp;

		/*for(int n=0; n<total; n++)
		{
			int a = (int) &movelist[n]; 
			HANDLE hTemp = CreateMutex(NULL,FALSE,(char*) a);
			vhTemp.push_back(hTemp);
			if(hTemp != NULL)
				moveMutexList.push_back(hTemp);
		}*/
	//	try{
		for(int n=0; n<movelist.size(); n++)
		{
			temppos.push_back(movelist[n]->GetPos());
			DInt diTemp;
			if(moveloc.a>=movelist[n]->GetPos().a && moveloc.b>=movelist[n]->GetPos().b)
			{
				diTemp.a=1;
				diTemp.b=1;
				DInt temp=GetModifiedShift(octSymbPos, movelist[n]->GetPos().a, movelist[n]->GetPos().b, moveloc.a, moveloc.b);
				xshift.push_back(temp.a);
				yshift.push_back(temp.b);
			}
			else if(moveloc.a<=movelist[n]->GetPos().a && moveloc.b<=movelist[n]->GetPos().b)
			{
				diTemp.a=-1;
				diTemp.b=-1;
				DInt temp=GetModifiedShift(octSymbPos, moveloc.a, moveloc.b, movelist[n]->GetPos().a, movelist[n]->GetPos().b);
				xshift.push_back(temp.a);
				yshift.push_back(temp.b);
			}
			else if(moveloc.a>=movelist[n]->GetPos().a && moveloc.b<=movelist[n]->GetPos().b)
			{
				diTemp.a=1;
				diTemp.b=-1;
				DInt temp=GetModifiedShift(octSymbPos, movelist[n]->GetPos().a,  moveloc.b, moveloc.a, movelist[n]->GetPos().b);
				xshift.push_back(temp.a);
				yshift.push_back(temp.b);
			}
			else if(moveloc.a<=movelist[n]->GetPos().a && moveloc.b>=movelist[n]->GetPos().b)
			{
				diTemp.a=-1;
				diTemp.b=1;
				DInt temp=GetModifiedShift(octSymbPos, moveloc.a, movelist[n]->GetPos().b, movelist[n]->GetPos().a, moveloc.b);
				xshift.push_back(temp.a);
				yshift.push_back(temp.b);
			}
			increm.push_back(diTemp);
			octSymbPos++;
		}
		
	
/*
for(int n=0; n<movelist.size(); n++)
		{
			temppos.push_back(movelist[n]->GetPos());
			DInt diTemp;
			if(moveloc.a>=movelist[n]->GetPos().a)
			{
				diTemp.a=1;
				if(g.a%2==0)
				{
					if(g.b%2==0 && moveloc.a-1>=0)
						xshift.push_back(moveloc.a-movelist[n]->GetPos().a-1);
					else if(g.b%2==1 && moveloc.a+1<50)
						xshift.push_back(moveloc.a-movelist[n]->GetPos().a+1);
				}
				else
					xshift.push_back(moveloc.a-movelist[n]->GetPos().a);
			}
			else 
			{
				diTemp.a=-1;
				if(g.a%2==0)
				{
					if(g.b%2==0 && moveloc.a-1>=0)
						xshift.push_back(movelist[n]->GetPos().a-moveloc.a-1);
					else if(g.b%2==1 && moveloc.a+1<50)
						xshift.push_back(movelist[n]->GetPos().a-moveloc.a+1);
				}
				else
					xshift.push_back(movelist[n]->GetPos().a-moveloc.a);
			}

			if(moveloc.b>=movelist[n]->GetPos().b)
			{
				diTemp.b=1;
				if(g.a%2==1)
				{
					if(g.b%2==0 && moveloc.b-1>=0 )
						yshift.push_back(moveloc.b-movelist[n]->GetPos().b-1);
					else if(g.b%2==1 && moveloc.b+1<50)
						yshift.push_back(moveloc.b-movelist[n]->GetPos().b+1);
				}
				else
					yshift.push_back(moveloc.b-movelist[n]->GetPos().b);
			}
			else 
			{
				diTemp.b=-1;
				if(g.a%2==1)
				{
					if(g.b%2==0 && moveloc.b-1>=0)
						yshift.push_back(movelist[n]->GetPos().b-moveloc.b-1);
					else if(g.b%2==1 && moveloc.b+1<50)
						yshift.push_back(movelist[n]->GetPos().b-moveloc.b+1);
				}
				else
					yshift.push_back(movelist[n]->GetPos().b-moveloc.b);
			}
			increm.push_back(diTemp);
			g.a++;
			g.b++;
		}*/
		MoveUnits(increm, xshift, yshift, temppos);
		Game::printmap();
		//KillThread();
		/*}
		catch(...)
		{
			cout<<"move process";
			system("pause");
			ExitThread(1);
		}*/
		return 0;
	}
	
}

DInt MoveThread::GetModifiedShift(int posSym, int smallPosA, int smallPosB, int bigPosA, int bigPosB)
{
	try{
	int posSymbol=posSym;
	for(;;)
	{
		DInt shiftTemp;
		switch(posSymbol%8)
		{
		case 0:
			if(smallPosA-1>=0)
				shiftTemp.a=bigPosA-smallPosA-1;
			else
			{
				posSymbol++;
				break;
			}
			if(smallPosB>=0)
				shiftTemp.b=bigPosB-smallPosB;
			else
			{
				posSymbol++;
				break;
			}
			return shiftTemp;
			break;
		case 1:
		case 2:
		case 3:
			if((smallPosA-2+posSymbol%8)>=0 && (smallPosA-2+posSymbol%8)<50)
				shiftTemp.a=bigPosA-smallPosA+2-posSymbol%8;
			else
			{
				posSymbol++;
				break;
			}
			if(smallPosB-1>=0)
				shiftTemp.b=bigPosB-smallPosB-1;
			else
			{
				posSymbol++;
				break;
			}
			return shiftTemp;
		case 4:
			if(smallPosA+1<50)
				shiftTemp.a=bigPosA-smallPosA+1;
			else
			{
				posSymbol++;
				break;
			}
			if(smallPosB>=0)
				shiftTemp.b=bigPosB-smallPosB;
			else
			{
				posSymbol++;
				break;
			}
			return shiftTemp;
			break;
		case 5:
		case 6:
		case 7:
			if(smallPosA+(6-posSymbol%8)>=0 && smallPosA+(6-posSymbol%8)<50)
				shiftTemp.a=bigPosA-smallPosA+(6-posSymbol%8);
			else
			{
				posSymbol++;
				break;
			}
			if(smallPosB+1<=0)
				shiftTemp.b=bigPosB-smallPosB+1;
			else
			{
				posSymbol++;
				break;
			}
			return shiftTemp;
			break;
		}
	}
	}

	catch(...)
	{
		Game::prints("hi");
		ExitProcess(-1);
	}
	//return shiftTemp;
}

void MoveThread::MoveUnits(vector<DInt> increm, vector<int> xshift, vector<int> yshift, vector<DInt> temppos)
{
		HANDLE moveTimer;
		moveTimer=CreateWaitableTimer(NULL, false, "MoveTimer");
		LARGE_INTEGER ref;
		ref.QuadPart=-100;
		const LARGE_INTEGER* large=&ref;

		int total=movelist.size();
		int counter=0;

		/*for(int n=0; n<total; n++)
		{
			HANDLE* temp;
			temp=find(moveMutexList.begin(), moveMutexList.end(), vhTemp.begin() + n);
			if(temp!=moveMutexList.end())
				WaitForSingleObject(*temp, 60000);
		}*/
		/*for(int n=0; n<total; n++)
			WaitForSingleObject(movelist[n]->GetMutex(), 30000);*/
		
		SetWaitableTimer(moveTimer,large,100, NULL, NULL, true);
		//try{
		do
		{
		if(WaitForSingleObject(moveTimer, INFINITE)==WAIT_OBJECT_0)
		{
			for(int n=0; n<movelist.size(); n++)
			{   
				if(counter%movelist[n]->GetSpeed()==0)
				{
					if(n<xshift.size() && xshift[n]>0 && temppos[n].a+increm[n].a>=0 && temppos[n].a+increm[n].a<=50)
					{
						xshift[n]-=1;
						temppos[n].a+=increm[n].a;
						Game::moveunit(movelist[n], temppos[n]);
					}
					if(n<yshift.size() && yshift[n]>0 && temppos[n].b+increm[n].b>=0 && temppos[n].b+increm[n].b<=50)
					{
						yshift[n]-=1;					
						temppos[n].b+=increm[n].b;
						Game::moveunit(movelist[n], temppos[n]);
					}	
					if(n<xshift.size() && n<yshift.size() && xshift[n]<=0 && yshift[n]<=0)
					{
						movelist.erase(movelist.begin() + n);
						temppos.erase(temppos.begin() + n);
						xshift.erase(xshift.begin() + n);
						yshift.erase(yshift.begin() + n);
						increm.erase(increm.begin() + n);
						total-=1;
					}
				}
			}
			counter++;
		} 

		}while (total>0);
//}
		/*catch(...)
		{
			cout<<"movethread moveunit  "<<GetLastError();
			system("pause");
			ExitThread(1);
			//ExitProcess(-1);
		}
	
		/*for(n=0; n<total; n++)
			ReleaseMutex(movelist[n]->GetMutex());*/

	/*	for(n=0; n<total; n++)
		{
			HANDLE* temp;
			temp=find(moveMutexList.begin(), moveMutexList.end(), vhTemp.begin() + n);
			if(temp!=moveMutexList.end())
				ReleaseMutex(*temp);
		}*/
		CancelWaitableTimer(moveTimer);
}

/*vector<HANDLE> href(0);
vector<HANDLE> MoveThread::moveMutexList = href;*/



//////////////////////////////////////////
//									     /
//AttackThread code                        /
//										 /
//////////////////////////////////////////

AttackThread::AttackThread(vector<Unit*> list, DInt loc, Unit* target)
{
	MoveThread::MoveThread(list, loc);
	movelist=list;
	moveloc=loc;
	unitTarget=target;
	buildingTarget=NULL;
}

AttackThread::AttackThread(vector<Unit*> list, DInt loc,  Building* target)
{
	MoveThread::MoveThread(list, loc);
	buildingTarget=target;
	unitTarget=NULL;
}

AttackThread::AttackThread()
{
	InputThread::InputThread();
}

AttackThread::~AttackThread()
{
	if(this->hThread) 
	{
		/*for(int n=0; n<movelist.size(); n++)
			movelist[n]=NULL;*/
		::TerminateThread (this->hThread, 1);
		::CloseHandle (this->hThread);
		//ExitProcess(0);
	}
}

bool AttackThread::CreateThread()
{
	MoveThread::CreateThread();
	return true;
}

unsigned long AttackThread::Process (void* parameter)
{
	//a mechanism for terminating thread should be implemented
	//not allowing the method to be run from the main thread
	if (::GetCurrentThreadId () == this->hMainThreadId)
		return 0;
	else 
	{
		//MoveThread::Process(parameter);
		int octSymbPos=1;

		vector<int> xshift, yshift;
		int total=movelist.size();
		vector<DInt> temppos;
		vector<DInt> increm;
		DInt g;
		g.a=0;
		g.b=0;
		
		for(int n=0; n<movelist.size(); n++)
		{
			temppos.push_back(movelist[n]->GetPos());
			DInt diTemp, tempLoc=moveloc;

			if(tempLoc.a>=movelist[n]->GetPos().a && tempLoc.b>=movelist[n]->GetPos().b)
			{
				diTemp.a=1;
				diTemp.b=1;
				
				DInt temp;
				//if(movelist[n]->GetRange()==1)
				temp=GetModifiedShift(octSymbPos, movelist[n]->GetPos().a, movelist[n]->GetPos().b, tempLoc.a, tempLoc.b);
				/*else
				{
					temp.a=tempLoc.a-movelist[n]->GetPos().a;
					temp.b=tempLoc.b-movelist[n]->GetPos().b;
				}*/temp.a+=floor(sqrt(pow(movelist[n]->GetRange(), 2)/2));
				temp.b+=floor(sqrt(pow(movelist[n]->GetRange(), 2)/2));
				xshift.push_back(temp.a);
				yshift.push_back(temp.b);
			}
			else if(tempLoc.a<=movelist[n]->GetPos().a && tempLoc.b<=movelist[n]->GetPos().b)
			{
				diTemp.a=-1;
				diTemp.b=-1;
				
				DInt temp;
				temp=GetModifiedShift(octSymbPos, tempLoc.a, tempLoc.b, movelist[n]->GetPos().a, movelist[n]->GetPos().b);
				temp.a-=floor(sqrt(pow(movelist[n]->GetRange(), 2)/2));
				temp.b-=floor(sqrt(pow(movelist[n]->GetRange(), 2)/2));
				xshift.push_back(temp.a);
				yshift.push_back(temp.b);
			}
			else if(tempLoc.a>=movelist[n]->GetPos().a && tempLoc.b<=movelist[n]->GetPos().b)
			{
				diTemp.a=1;
				diTemp.b=-1;
				
				DInt temp;
				temp=GetModifiedShift(octSymbPos, movelist[n]->GetPos().a,  tempLoc.b, tempLoc.a, movelist[n]->GetPos().b);
				temp.a+=floor(sqrt(pow(movelist[n]->GetRange(), 2)/2));
				temp.b-=floor(sqrt(pow(movelist[n]->GetRange(), 2)/2));
				xshift.push_back(temp.a);
				yshift.push_back(temp.b);
			}
			else if(tempLoc.a<=movelist[n]->GetPos().a && tempLoc.b>=movelist[n]->GetPos().b)
			{
				diTemp.a=-1;
				diTemp.b=1;
				
				DInt temp;
				temp=GetModifiedShift(octSymbPos, tempLoc.a, movelist[n]->GetPos().b, movelist[n]->GetPos().a, tempLoc.b);
				tempLoc.a-=floor(sqrt(pow(movelist[n]->GetRange(), 2)/2));
				tempLoc.b+=floor(sqrt(pow(movelist[n]->GetRange(), 2)/2));
				xshift.push_back(temp.a);
				yshift.push_back(temp.b);
			}
			increm.push_back(diTemp);
			octSymbPos++;
		}
		AttackUnits(increm, xshift, yshift, temppos);
		Game::printmap();
		//AttackUnits();
		return 0;
	}
	
}

void AttackThread::AttackUnits(vector<DInt> increm, vector<int> xshift, vector<int> yshift, vector<DInt> temppos)
{
		HANDLE attackTimer;
		attackTimer=CreateWaitableTimer(NULL, false, "attack_timer");
		LARGE_INTEGER ref;
		ref.QuadPart=-100;
		const LARGE_INTEGER* large=&ref;

		//int total=movelist.size();
		int timeCounter=0;
		//Game::prints("hi");
		srand(time(NULL));
		SetWaitableTimer(attackTimer,large,100, NULL, NULL, true);

		if(unitTarget!=NULL)
		{
			do
			{
			if(WaitForSingleObject(attackTimer, INFINITE)==WAIT_OBJECT_0)
			{
				for(int n=0; n<movelist.size(); n++)
				{   
					if(Distance(unitTarget->GetPos(), movelist[n]->GetPos())>movelist[n]->GetRange())
					{
						if(timeCounter%movelist[n]->GetSpeed()==0)
						{
							if(n<xshift.size() && xshift[n]>0 && temppos[n].a+increm[n].a>=0 && temppos[n].a+increm[n].a<=50)
							{
								xshift[n]-=1;
								temppos[n].a+=increm[n].a;
								Game::moveunit(movelist[n], temppos[n]);
							}
							if(n<yshift.size() && yshift[n]>0 && temppos[n].b+increm[n].b>=0 && temppos[n].b+increm[n].b<=50)
							{
								yshift[n]-=1;					
								temppos[n].b+=increm[n].b;
								Game::moveunit(movelist[n], temppos[n]);
							}	
							if(n<xshift.size() && n<yshift.size() && xshift[n]<=0 && yshift[n]<=0)
							{
								//movelist.erase(movelist.begin() + n);
								temppos.erase(temppos.begin() + n);
								xshift.erase(xshift.begin() + n);
								yshift.erase(yshift.begin() + n);
								increm.erase(increm.begin() + n);
								//total-=1;
							}
						}
					}
					else
					{
						if(timeCounter%movelist[n]->GetAttackRate()==0)
						{
							int z=(rand()%20+movelist[n]->GetAttack());
							//Game::prints("Attack roll: "+ Game::ToString(z) + "  Armor: " + Game::ToString(unitTarget->GetArmor()));
							if(z > unitTarget->GetArmor())
							{
								//z=movelist[n]->GetDamage().a+(rand()%(movelist[n]->GetDamage().b-movelist[n]->GetDamage().a));
								//Game::prints("Damage: "+Game::ToString(z));
								unitTarget->SubHp(movelist[n]->GetDamage().a+(rand()%(movelist[n]->GetDamage().b-movelist[n]->GetDamage().a)));
								if(unitTarget->GetHp().a<=0)
								{
									Game::prints("We have killed enemy unit: " + unitTarget->GetName());
									movelist.clear();
									ExitThread(1);
								}
							}
						}
					}
				}
				timeCounter++;
			} 

			}while (unitTarget->GetHp().a>0);
		}
		/*else if(buildingTarget!=NULL)
		{
			do
			{
			if(WaitForSingleObject(attackTimer, INFINITE)==WAIT_OBJECT_0)
			{
				for(int n=0; n<total; n++)
				{
					//if(movelist[n])

				}
				timeCounter++;
			} 

			}while (buildingTarget->GetHP().a>0);

			if(buildingTarget->GetHP().a<=0)
				buildingTarget->Alive(false);
		}*/


			/*do
			{
			
			if(WaitForSingleObject(attackTimer, INFINITE)==WAIT_OBJECT_0)
			{	//unitTarget->SubHp(10);
				//cout<<total;
				//timeCounter++;
				for(int n=0; n<total; n++)
				{
					if(timeCounter%movelist[n]->GetAttackRate()==0)
					{//cout<<timeCounter;
						//Game::prints("Begin attack for unit");
						int z=(rand()%20+movelist[n]->GetAttack());
						//Game::prints("Attack roll: "+ Game::ToString(z) + "  Armor: " + Game::ToString(unitTarget->GetArmor()));
						if(z > unitTarget->GetArmor())
						{
							z=movelist[n]->GetDamage().a+(rand()%(movelist[n]->GetDamage().b-movelist[n]->GetDamage().a));
							//Game::prints("Damage: "+Game::ToString(z));
							unitTarget->SubHp(movelist[n]->GetDamage().a+(rand()%(movelist[n]->GetDamage().b-movelist[n]->GetDamage().a)));
							if(unitTarget->GetHp().a<=0)
							{
								Game::prints("We have killed enemy unit: " + unitTarget->GetName());
								break;
							}
						}
					}
				}

				timeCounter++;
			} 

			}while (unitTarget->GetHp().a>0);
		}
		*/
		movelist.clear();
		CancelWaitableTimer(attackTimer);
		//KillThread();
}