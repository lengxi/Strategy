
#if !defined(UREF)
#define UREF

#ifndef UNIT
#include "Unit.h"
#endif
#ifndef DINT
#include "DInt.h"
#endif

//Unit(double hit, int atk, int atkrate, int damlow, int damhigh, int rng, int spd, 
//            string nm, string des, int posx, int posy, int ac);

Unit rfoot(400, 2, 6, 12, 14, 1, 1, "Footman", "d", 2, 2, 1);//5,5,5,5,5,"h","h",6,6,6);
Unit rrifle(450, 5, 9, 18, 22, 5, 2, "Rifleman", "d", 2, 3, 0);//5,5,5,5,5,"h","h",6,6,6);
Unit rknight(90, 9, 6, 28, 34, 1, 3, "Knight", "d", 20, 40, 0);//5,5,5,5,5,"h","h",6,6,6);

int rftime ;
int rrtime ;
int rktime ;

int rbatime ;
int rbltime ;
int rsttime ;

DInt rfcost={0,0};
DInt rrcost={0,0};
DInt rkcost={0,0};

DInt rbacost;
DInt rblcost;
DInt rstcost;


#endif

/*for(int a=0; a<50; a++)
	{
		locate(0,a);
		for(b=0; b<50; b++)
		{
			cout<<bgrid[a][b];
		}
		cout<<endl;
	}*/

	//locate(0,0);
	/*for(a=0; a<50; a++)
	{
		for(int b=0; b<50; b++)
		{
			if(ugrid[a][b]!=NULL)
				cout<<ugrid[a][b]->GetName()[0];
			else
				locate(b+1,a);
		}
		cout<<endl;
	}*/
	
	//prints the units
	/*for(a=0; a<unitlist.size(); a++)
	{
		locate(unitlist[a].GetPos().a,unitlist[a].GetPos().b);

		if(unitlist[a].GetName()=="Footman")
		{
			if(unitlist[a].GetHp().a/unitlist[a].GetHp().b<=0.33)
			{
				setcolor(RED);
				cout<<"F";
				setcolor(LGRAY);
			}
			else if(unitlist[a].GetHp().a/unitlist[a].GetHp().b>=0.66)
			{
				setcolor(LBLUE);
				cout<<"F";
				setcolor(LGRAY);
			}
			else
			{
				setcolor(YELLOW);
				cout<<"F";
				setcolor(LGRAY);
			}
		}

		else if(unitlist[a].GetName()=="Rifleman")
		{
			if(unitlist[a].GetHp().a/unitlist[a].GetHp().b<=0.33)
			{
				setcolor(RED);
				cout<<"K";
				setcolor(LGRAY);
			}
			else if(unitlist[a].GetHp().a/unitlist[a].GetHp().b>=0.66)
			{
				setcolor(LGRAY);
				cout<<"K";
			}
			else
			{
				setcolor(YELLOW);
				cout<<"K";
				setcolor(LGRAY);
			}
		}
		else if(unitlist[a].GetName()=="Knight")
		{
			if(unitlist[a].GetHp().a/unitlist[a].GetHp().b<=0.33)
			{
				setcolor(RED);
				cout<<"K";
				setcolor(LGRAY);
			}
			else if(unitlist[a].GetHp().a/unitlist[a].GetHp().b>=0.66)
			{
				setcolor(LGRAY);
				cout<<"K";
			}
			else
			{
				setcolor(YELLOW);
				cout<<"K";
				setcolor(LGRAY);
			}
		}
	}

	for(a=0; a<buildinglist.size(); a++)
	{
		locate(buildinglist[a].GetPos().a, buildinglist[a].GetPos().b);
		//if(buildinglist[a].getname()=="Barracks")
		//{
			if(buildinglist[a].GetHP().a/buildinglist[a].GetHP().b<=0.1)
			{
				setcolor(DRED);
				cout<<buildinglist[a].getname()[0];
				setcolor(LGRAY);
			}
			else 
			{
				setcolor(DSBLUE);
				cout<<buildinglist[a].getname()[0];
				setcolor(LGRAY);
			}
	}
		//}



  //threads original code
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
		}
*/

