// Game.h: interface for the Game class.
#if !defined(GAME)
#define GAME

//#ifndef UNIT
#include "Unit.h"
//#endif
//#ifndef BUILDING
#include "Building.h"
//#endif
//#ifndef MAP
#include "Map.h"
//#endif
//#ifndef UREF
//#endif

#include "threads.h"
//#endif
#include <vector>

typedef vector<Unit*> UVector;


class Game  
{
public:
	static void ClearHoverArea();
	static void MoveMouseCursor(int x, int y);
	static void movecursor(int x, int y);
	inline static Map* getmap();
	static void moveunit(Unit* munit, DInt mpos);
	static Building* GetBuilding(string nm);
	static Building* GetBuilding(DInt pos);
	static vector<Unit*> GetUnit(DInt pos);

	template <class T> inline static void prints(T a)
	{
		WaitForSingleObject(cmdout, INFINITE);
		locate(0, print++);
		cout<<a;
		ReleaseMutex(cmdout);	
	};

	inline static void prints();

	static string ToString(int a);

	static void OnSelect(DInt curpos);

	static void OnSelectGroup(int groupnumber);

	static void OnGroup(COORD topLeft, COORD botRight);

	static void OnMove(COORD movePos);

	static void OnAttack(COORD movePos);

	static void OnTrain();

	static void OnBuild();

	void startgame();

	Game();
	virtual ~Game();

	int getkeyO();
	static int getkeyA();

	static DInt cursor;
	static void locate(int x, int y);

    static void printmap();

	static HANDLE cmdout;
	static int print;

private:
	void OnTurn();

	static int gold;
	static int wood;
	static int mcount;
	//DInt pop;
	
	//int unitcount;
	static vector<Unit> units;
	static vector<Unit> wisps;
	static vector<UVector> groups;
	static vector<Unit*> selected;
	//int freeswisp;
	//int goldswisp;
	//int woodswisp;
	//int soulswisp;

	//Queue* queues[5];


	InputThread keyinput;

	static Building towncenter;
	//static Building barracks;
	static Building blacksmith;
	static Building stables;
	//Building farm;
	static vector<Building> buildings;

	static Map map;
	//char cgrid[50][50];

	static HANDLE m_Keyboard;
	static HANDLE m_screen;
	static HANDLE threadCreationMutex;
	//static vector<MoveThread*> moveThreadPointerList; 
	static vector<MoveThread> moveThreads;
	static vector<AttackThread> attackThreads;
	//static vector<AttackThread*> attackThreadPointerList;

	
};

#endif 