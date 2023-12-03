#ifndef ___MONSTER_H___
#define ___MONSTER_H___
#include "c\obj.h"
#include <stdio.h>

#define MIN_MONSTER_TYPE 12 // (12*8)=96 - с 96-й модели - монстры (8 фаз)
#define MONST_TYPES 8

enum{ CND_RAND=0,CND_CHASE,CND_WAIT,CND_TELE,CND_SLIP,CND_DUPL,
	CND_RAGE=254,CND_SHOCK=255
};

struct TMonster:TBeing{
	int cnd,type,count,turn,age;
	char fzStep,fzFire;
	int n_man,man_hol,a2man,r2man;
	TMonster();
	int Die(int);
	int Move();
	void Sprite(int);
	int ManVis();
	void GetNearMan();
	void ChangeCnd();
	void Fire();
	void Save(int);
	void Step();
	int Give(int what,int a=0,int b=0);
	inline void BeginTurn(int,int);
	void TurnToMan();
};

struct TIncubator:TObj{
	char type,speed;
	int age;
	TIncubator();
	void Sprite(int);
	int Move();
	int Its(){ return ITS_INCUBATOR; }
	void Save(int);
};
#endif
