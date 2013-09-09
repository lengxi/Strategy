#define WIN32_LEAN_AND_MEAN
#define NOKANJI
#define NOSOUND
#define NOHELP 

//#include <iostream>

#include "Game.h"
//#include "wincon.h"

//using namespace std;


 
int main()
{
//	char a;
	//setting cursor to be 1% of a square
	CONSOLE_CURSOR_INFO cursorinfo;
	cursorinfo.dwSize=1;
	cursorinfo.bVisible=true;
	if(!SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorinfo))
		cout<<"Error setting cursor size. Bypassing..."<<endl;

	//setting console screen to be 67 by 79
	SMALL_RECT srTemp;
	srTemp.Top=0; 
	srTemp.Left=0;
	srTemp.Bottom=67;
	srTemp.Right=79;
	CONST SMALL_RECT screenDim=srTemp;
	if(!SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &screenDim)) 
		cout<<"Error setting window size. Bypassing..."<<endl;

	system("cls");
	cout<<"\n\n";
	cout<<"                              Strategy"<<endl;
	cout<<"                              ASCII RTS"<<endl<<endl;
	cout<<"                              By lengxi"<<endl<<endl<<endl<<"\t\t   ";

	/*cout<<"                     Press enter to begin... ";
	cin>>a;*/
	system("pause");

	system("cls");

	//start the game with a new game object
	Game mygame;
	
	cout<<endl<<endl<<"Beginning game... ";
	system("cls");

	
	mygame.startgame();

	return 0;
}
