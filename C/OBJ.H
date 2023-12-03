#ifndef ___OBJ_H___
#define ___OBJ_H___

#include "c\defs.h"
#define MAX_HEALTH 3200
#define MAX_ARMOR 12672
#define MAX_RENT   6336
#define MAX_SLIP   1584

struct TBeing;
struct TVect;

struct TManInf{
	int number,health,slip,rent,armor,invis,radar,cash,nKill;
	int nBuyWea,isOk;
	int ammo[NUM_WEAP];
	long doh;
	TManInf(){}
	void New(int);
	void MakeMan();
	void Buy0();
	void Buy1();
	void Buy2();
	void Give();
	void Sell();
	void GetInfo();
	void CashSell(int plus){
		cash+=plus;
		if(cash>30000 || cash<0) cash=30000;
	}
};

struct TObj{
	int x,y,r;
	TObj(){ x=x_obj; y=y_obj; }
	int R(TObj &O){ r=Distance(x-O.x,y-O.y); return r; }
	inline int GetAngle(TVect &O);
	inline int Direction(TObj &O);
	virtual int Die(int){ return 0; }
	virtual void Push(int,int){}
	virtual int Take(TBeing &Man){ return NOTAKE; }
	virtual int Move(){ return OK; }
	virtual void Sprite(int)=0;
	virtual void Save(int){}
	virtual int Its(){ return 0; }
	virtual int Burst(){ return DELETE; }
	virtual void Name(){}
	int Into(int dx,int dy){
		return
			(world[(x+dx)>>6][(y+dy)>>6]!=0) ||
			(world[(x+(dx>>1))>>6][(y+(dy>>1))>>6]!=0);
	}
	void InSand();
};

struct TVect:TObj{
	int angle;
	TVect():TObj(){ angle=ang_obj; }
	void Turn(int da){
		angle+=da;
		if(angle>=ANGLE_360) angle-=ANGLE_360;
		else if(angle<ANGLE_0) angle+=ANGLE_360;
	}
	int dAngle(int da){
		int a=angle+da; if(a>=ANGLE_360) a-=ANGLE_360;
		else if(a<ANGLE_0) a+=ANGLE_360;
		return a;
	}
	int TurnToTarget(int);
	int TargIsVis(int);
};

struct TMov:TVect{
	int mv_angle,mv_speed;
	TMov():TVect(){ mv_angle=mv_speed=0; }
	void Push(int a,int s){
		mv_angle=a+ANGLE_180;
		if(mv_angle>=ANGLE_360) mv_angle-=ANGLE_360;
		mv_speed=-s;
	}
	int WallClip();
	virtual void Step();
	virtual void Telep(int X,int Y);
	void RndTelep();
};

struct TBeing:TMov{
	char nEnemy,fzDead,_life,pain;
	int speed,health,number;
	TBeing():TMov(){
		health=MAX_HEALTH; number=n_obj;
		fzDead=speed=0; _life=1; nEnemy=255;
	}
	void SlipStep();
	virtual void Step();
	virtual int Die(int)=0;
	virtual int Give(int what,int a=0,int b=0){ return NOTAKE; }
	int Its(){ return fzDead?0:ITS_MAN; }
	void Telep(int X,int Y);
	void Destroy();
	void Bits(char);
	void Blood(int speed,int time);
};

struct TMan:TBeing{
	char radar,fzStep;
	int ang,slip,rent,armor,invis,cash,nWeap,w_sp,fzWeap,nKill,strcnt,nInfStr;
	int SCR_Y_MID,SCR_X_MIN;
	int ammo[NUM_WEAP];
	long doh;
	TMan();
	void Dead(){ if(!fzDead) fzDead=1; health=0; KILLED[number]++; }
	void Cash(int plus){
		cash+=plus;
		if(cash>30000 || cash<0) cash=30000;
		doh+=plus;
		if(doh>2000000000L) doh=2000000000L;
	}
	int Move();
	int Take(TBeing&){ return fzDead?NOTAKE:STOP; }
	void RayCast();
	void DrawOpp();
	int ObjIsVis(int ang);
	void ChangeWeap();
	void Clear(){ tex_x=SCR_X_MIN; tex_top=SCR_Y_MID-4; CLEAR(); }
	void KIR(); void GUN(); void PLM(); void SHK();
	void TAZ(); void ROC(); void MAV(); void MIN();
	void BLD(); void HOL(); void TEL();
	void Draw0(); void Draw1();
	void CalcMaxSpeed();
	void Inp0(); void Inp1(); void Inp2(); int Die(int power);
	void Sprite(int);
	int Give(int what,int a=0,int b=0);
	void DrawMap();
	void DrawMap(int i,int j);
	void View(){ Clear(); RayCast(); DrawOpp(); }
	void Info(int n){ nInfStr=n; strcnt=60; }
	void Resurrect();
	void StateInf();
	void Step();
	void GetInfo(TManInf &M);
	void Save(int);
	void FindWeap();
	void Agony();
   void AddMine();
	void ExplTelep();
	void DropWeap();
};

struct THol:TVect{
	int vis;
	int num;
	THol():TVect(){ vis=0; num=n_obj; }
	void Sprite(int);
};

struct TArm:TVect{
	int vis;
	char num,n_hol;
	TArm():TVect(){ vis=t_obj; num=n_obj; n_hol=t_obj; }
	void Sprite(int);
};

struct T1aid:TObj{
	T1aid():TObj(){}
	int Take(TBeing &Man){ return Man.Give(100); }
	void Sprite(int);
	void Save(int);
};

struct TItem:TObj{
	char type;
	TItem():TObj(){ type=t_obj; }
	int Take(TBeing &M){ return M.Give(type); }
	void Sprite(int);
	void Save(int);
};

struct TGold:TObj{
	char price;
	TGold():TObj(){ price=t_obj; }
	int Take(TBeing &M){ return M.Give(99,price); }
	void Sprite(int);
	void Save(int);
};

struct TBarrel:TMov{
	int type;
	TBarrel():TMov(){ type=t_obj; }
	void Sprite(int);
	int Move();
	int Its(){ return ITS_THING; }
	void Save(int);
};

struct TCorpse:TVect{
	int n,cnt;
	TCorpse():TVect(){ n=n_obj; cnt=0; }
	void Sprite(int);
	void Save(int);
};

struct TBullet:TVect{
	char nOwner;
	TBullet();
	int Fly(int);
};

struct TGren:TBullet{
	int h,time;
	TGren():TBullet(){ h=h_obj; time=t_obj; }
	virtual int Move()=0;
	int Its(){ return ITS_RC; }
};

struct TBarrelObl:TBullet{
	TBarrelObl():TBullet(){}
	int Burst();
	void Sprite(int);
	int Move();
	int Its(){ return ITS_RC; }
	void Save(int);
};

struct TPul:TBullet{
	char pow;
	TPul():TBullet(){ pow=pow_obj; }
	int Move();
	int Burst();
	void Sprite(int);
	void Save(int);
};

struct TMis:TBullet{
	TMis():TBullet(){}
	void Sprite(int);
	int Burst();
	int Move();
	int Its(){ return ITS_RC; }
	void Save(int);
};

struct TNap:TBullet{
	int time;
	TNap():TBullet(){ time=t_obj; }
	int Move();
	int Burst();
	void Sprite(int);
	void Save(int);
};

struct TTaz:TBullet{
	char type,wait;
	TTaz():TBullet(){ type=t_obj; wait=1; }
	int Burst();
	void Sprite(int);
	int Move();
	int Its(){ return ITS_RC; }
	void Save(int);
};

struct TWeap:TVect{
	int type,ammo,h,speed;
	TWeap():TVect(){ type=t_obj; ammo=r_obj; h=h_obj; speed=n_obj; }
	int Take(TBeing &M){ return M.Give(0,ammo,type); }
	int Move();
	void Sprite(int);
	void Save(int);
};

struct TBit:TVect{
	int h,speed,time,type;
	TBit():TVect(){ time=t_obj; type=n_obj; speed=pow_obj; h=-9500; }
	void Sprite(int);
	int Move();
	void Save(int);
};

struct TMine:TGren{
	TMine():TGren(){}
	int Move();
	int TargetIsVis();
	void Sprite(int);
	void Save(int);
};

struct TMav:TBullet{
	int timer;
	TMav():TBullet(){ timer=0; }
	int Burst();
	void Sprite(int);
	int Move();
	int Its(){ return ITS_RC; }
	void Save(int);
};

struct TShock:TVect{
	char nOwner,fz;
	TShock():TVect(){ nOwner=own_obj; fz=5; }
	int Move();
	void Sprite(int);
	void Save(int);
};

struct TExpl:TObj{
	char nOwner,fz,radius,type;
	int size,power;
	TExpl();
	int Its(){ return ITS_EXPL; }
	void Sprite(int);
	int Move();
	void Expl();
	void Save(int);
};

struct TMultExpl:TBullet{
	char faze,radius,length,type;
	int size,power;
	TMultExpl():TBullet(){
		radius=r_obj; power=pow_obj; faze=fz_obj;
		size=sz_obj; length=n_obj; type=t_obj;
	}
	void Sprite(int){ tex_show=99; }
};

struct TNExpl:TMultExpl{
	TNExpl():TMultExpl(){}
	int Move();
	void Save(int);
};

struct TLiqExpl:TMultExpl{
	TLiqExpl():TMultExpl(){}
	int Move();
	void Save(int);
};
extern void(TManInf::*ManInfBuy[3])();
extern void(TMan::*ManInput[3])();
#endif