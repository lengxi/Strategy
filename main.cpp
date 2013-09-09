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
	cout<<"                              Valinor"<<endl;
	cout<<"                     A strategic adventure in DOS"<<endl<<endl;
	cout<<" Set in a land untouched by time and the hands of the second-born..."<<endl;
	cout<<"    Beyond the westerning shore, towards the starlight undying..."<<endl;
	cout<<"     For Morgoth and Ungoliant, and the glory of the fell storm!"<<endl<<endl<<endl;
	cout<<"Cast upon the free peoples by Darksyde | Obsidian Studios and ArkonSoft"<<endl<<endl;
	cout<<"                  Fed by Cogency Software Solutions"<<endl<<endl<<endl<<"\t\t   ";

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