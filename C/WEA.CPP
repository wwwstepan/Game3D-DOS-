#include "c\obj.h"
#include "c\wea.h"
#include "c\debug.h"
#include <stdlib.h>

// int TBullet::Fly(int radius=18){
// int TBarrel::Move(){
// int TMine::TargetIsVis(){
// int TBarrelObl::Move(){
// int TPul::Move(){
// int TVect::TargIsVis(int a2t){
// int TVect::TurnToTarget(int turn){
// int TMis::Move(){
// int TTaz::Move(){
// int TMav::Move(){
// int TNap::Move(){
// int TBit::Move(){
// int TWeap::Move(){
// int TMine::Move(){
// int TShock::Move(){
// TExpl::TExpl():TObj(){
// int TExpl::Move(){
// void TExpl::Expl(){
// int TNExpl::Move(){
// int TLiqExpl::Move(){
// int TPul::Burst(){
// int TNap::Burst(){
// int TMis::Burst(){
// int TTaz::Burst(){
// int TMav::Burst(){
// int TBarrelObl::Burst(){

int TBullet::Fly(int radius=18){
	for(register int i=0; i<=NUM_OBJ; i++)
		if(R(*Obj[i])<radius && Obj[i]->Its()>29 && Obj[i]!=this)
			return Burst();
	return OK;
}

int TBarrel::Move(){
	int j; TExpl *Ex; Step();
	for(register int i=0; i<=NUM_OBJ; i++)
		if((R(*Obj[i])<50) && (Obj[i]->Its()==ITS_EXPL)){
			Ex=(TExpl*)Obj[i];
			if(((rand()&31)==0)||(Ex->type&EXPL_TEL)){
				x_obj=x; y_obj=y; own_obj=Ex->nOwner;
				if(type<3){
					ang_obj=0; t_obj=300+(rand()&255); AddObj(new TNap);
					ang_obj+=ANGLE_90; t_obj=300+(rand()&255); AddObj(new TNap);
					ang_obj+=ANGLE_90; t_obj=300+(rand()&255); AddObj(new TNap);
					ang_obj+=ANGLE_90; t_obj=300+(rand()&255); AddObj(new TNap);
				}else{
					int num=6+(rand()&7);
					for(j=0; j<num; j++){
						ang_obj=random(ANGLE_360);
						t_obj=rand()&1; AddObj(new TBarrelObl);
					}
					t_obj=1; r_obj=100;
					num=3+rand()&7;
					for(j=0; j<num; j++){
						x_obj=x+(rand()&63)-31; y_obj=y+(rand()&63)-31;
						fz_obj=5+(rand()&3); pow_obj=POW_BARREL+(rand()&15);
						sz_obj=8000+(rand()&4096);
						AddObj(new TExpl);
					}
				}
				return DELETE;
			}
		}
	return OK;
}

int TMine::TargetIsVis(){
	int X=x,Y=y,COSa=COS(angle)>>4,SINa=SIN(angle)>>4;
	for(register int n=r>>5; n; n--){
		X+=COSa; Y+=SINa;
		if(world[X>>6][Y>>6]) return 0;
	}return 1;
}

int TBarrelObl::Move(){
	Turn((rand()&15)-7);
	int sp=16+(rand()&7);
	int dx=(COS(angle)*sp)>>9,dy=(SIN(angle)*sp)>>9;
	if(Into(dx,dy)) return Burst();
	x+=dx; y+=dy; return Fly(40);
}

int TPul::Move(){
	int in,dx=COS(angle)>>4,dy=SIN(angle)>>4;
	in=Into(dx,dy);
	if(in){ Burst(); x+=dx; y+=dy; InSand(); return DELETE; }
	x+=dx; y+=dy;
	if(Fly()!=OK) return Burst();
	dx=COS(angle)>>4,dy=SIN(angle)>>4;
	in=Into(dx,dy);
	if(in){ Burst(); x+=dx; y+=dy; InSand(); return DELETE; }
	x+=dx; y+=dy; //Turn((rand()&1)?1:-1); // поворот!
	return Fly();
}

int TVect::TargIsVis(int a2t){
	int X=x,Y=y,COSa=COS(a2t)>>4,SINa=SIN(a2t)>>4;
	for(register int n=r>>5; n>1; n--){
		X+=COSa; Y+=SINa;
		if(world[X>>6][Y>>6])
			return 0;
	}return 1;
}

int TVect::TurnToTarget(int turn){
	int sp=0,min_a2t=30000;
	for(int i=0; i<=NUM_OBJ; i++)
		if(Obj[i]->Its()==ITS_MAN){
			int d2t=Direction(*Obj[i]);
			int a2t=d2t-angle;
			if(a2t<=-ANGLE_180) a2t+=ANGLE_360;
			if(abs(a2t)<abs(min_a2t)){
				R(*Obj[i]);
				if(TargIsVis(d2t)) min_a2t=a2t;
			}
		}
	int abs_min_a2t=abs(min_a2t);
	if(abs_min_a2t<turn){ sp=2; Turn(min_a2t); }
	else if(abs_min_a2t>ANGLE_150) sp=1;
	else{
		if(min_a2t>0) Turn(turn);
		else if(min_a2t<0) Turn(-turn);
		else sp=3;
	}
	return sp;
}

int TMis::Move(){
	static int speed_turn[]={9,12,18,22};
	x_obj=x; y_obj=y; r_obj=30; pow_obj=3; t_obj=0;
	sz_obj=2600; own_obj=nOwner; fz_obj=2;
	int sp=speed_turn[TurnToTarget(4)];
	if(sp>15) fz_obj+=3;
	AddObj(new TExpl);
	int dx=(COS(angle)*sp)>>9,dy=(SIN(angle)*sp)>>9;
	if(Into(dx,dy)) return Burst(); x+=dx; y+=dy; return Fly(35);
}

int TTaz::Move(){
	if(--type<1) return DELETE;
	int sp=11+(rand()&7);
	if(sp<15) TurnToTarget(6);
	else Turn( wait ? ((rand()&7)-4) : ((rand()&31)-15) );
	int dx=(COS(angle)*sp)>>8,dy=(SIN(angle)*sp)>>8;
	///// Teleportation /////
	char w1=world[(x+dx)>>6][(y+dy)>>6];
	char w2=world[(x+(dx>>1))>>6][(y+(dy>>1))>>6];
	if(w1>=128){
		char wr_tel=w1-128;
		x=(((int)tel_w_x[wr_tel])<<6)+32;
		y=(((int)tel_w_y[wr_tel])<<6)+32;
		if(x>64 && y>64 && x<(WORLD_X_SIZE-64) && y<(WORLD_Y_SIZE-64))
			world[x>>6][y>>6]=0;
		else return DELETE;
		x_obj=x; y_obj=y; own_obj=nOwner;
		r_obj=27; pow_obj=33; t_obj=0; sz_obj=5000; fz_obj=6; AddObj(new TExpl);
	}else{
		if(w1>=WALL_SAND || w2>=WALL_SAND) type+=10;
	}
	if(w1 || w2){
		Burst();
		angle=(rand()&3)*ANGLE_90+ANGLE_45;
		return OK;
	}
	x+=dx; y+=dy;
	if(!wait) return Fly(25);
	wait--; return OK;
}

int TMav::Move(){
	if(timer<2) timer++;
	else{
		x_obj=x; y_obj=y; ang_obj=dAngle(ANGLE_180); r_obj=30; pow_obj=5; t_obj=0;
		fz_obj=3; sz_obj=4000; n_obj=6; own_obj=nOwner; AddObj(new TLiqExpl);
	}
	x+=(COS(angle)>>4); y+=(SIN(angle)>>4);
	return (world[x>>6][y>>6])?Burst():Fly(45);
}

int TNap::Move(){
	if(--time<0) return DELETE;
	int dx=(COS(angle)*3)>>6,dy=(SIN(angle)*3)>>6;
	if(Into(dx,dy)) angle=(rand()&3)*ANGLE_90; else{ x+=dx; y+=dy; }
	return Fly(50);
}

int TBit::Move(){
	if(h<4500){
		int dx=(COS(angle)*speed)>>9,dy=(SIN(angle)*speed)>>9;
		h+=1100; if(!Into(dx,dy)) x+=dx,y+=dy;
	}return (--time<1)?DELETE:OK;
}

int TWeap::Move(){
	if(h<1000){
		int dx=(COS(angle)*speed)>>9,dy=(SIN(angle)*speed)>>9;
		h+=1250; if(!Into(dx,dy)) x+=dx,y+=dy;
	}return OK;
}

int TMine::Move(){
	int dx=(COS(angle)*3)>>6,dy=(SIN(angle)*3)>>6;
	if(h<4500){ h+=1300; if(!Into(dx,dy)){ x+=dx; y+=dy; } return OK; }
	if(time>0){ time--; return OK; }
	for(int i=0; i<=NUM_OBJ; i++){
		int its=Obj[i]->Its();
		if(Obj[i]!=this && Obj[i]->R(*this)<200 && (its==ITS_RC || its==ITS_MAN)){
			if(its==ITS_MAN){
				TMan *M=(TMan*)Obj[i];
				if(M->number==nOwner) continue; // на владельца мины не реагируем
			}
			angle=Direction(*Obj[i]); r=Obj[i]->R(*this);
			if(TargetIsVis()){
				x_obj=x; y_obj=y; ang_obj=angle; r_obj=125; pow_obj=POW_MINE;
				t_obj=EXPL_TEL; fz_obj=6; sz_obj=15000; n_obj=8;
				own_obj=nOwner; AddObj(new TNExpl);
				return DELETE;
			}
		}
	}
	return OK;
}

int TShock::Move(){
	InSand();
	if(fz==2){
		for(int i=0; i<=NUM_OBJ; i++){
			if(R(*Obj[i])<55 && Obj[i]->Its()>31){
				ang_obj=ANGLE_20+random(ANGLE_90);
				if(rand()&1) ang_obj=-ang_obj;
				x_obj=x+(COS(ang_obj)>>3); y_obj=y+(SIN(ang_obj)>>3);
				int nSh=5+(rand()&7);
				while(nSh--){
					ang_obj+=(rand()&128)-63;
					if(ang_obj<0) ang_obj+=ANGLE_360;
					else if(ang_obj>ANGLE_360) ang_obj-=ANGLE_360;
					x_obj+=(COS(ang_obj)>>4); y_obj+=(SIN(ang_obj)>>4);
					if(x_obj<0 || y_obj<0 || x_obj>=WORLD_X_SIZE || y_obj>=WORLD_Y_SIZE) break;
					AddObj(new TShock);
					if(world[x_obj>>6][y_obj>>6]) break;
				}
				x_obj=x; y_obj=y; r_obj=65; pow_obj=POW_SHOCK+(rand()&15);
				t_obj=EXPL_INV; sz_obj=1000; own_obj=nOwner;
				fz_obj=3; AddObj(new TExpl);
			}
		}
	}return --fz>0?OK:DELETE;
}

TExpl::TExpl():TObj(){
	fz=fz_obj; radius=r_obj; power=pow_obj;
	size=sz_obj; type=t_obj; nOwner=own_obj;
	if(!(type&EXPL_NMG)) Expl();
}

int TExpl::Move(){
	if(type&EXPL_NMG) Expl();
	return(--fz<'\xA')?OK:DELETE;
}

void TExpl::Expl(){
	for(int i=0; i<=NUM_OBJ; i++)
		if(R(*Obj[i])<radius && Obj[i]->Its()==ITS_MAN){
			TBeing *M=(TBeing*)Obj[i];
			register int m_n=M->number;
			if((!M->fzDead) && (M->_life<100 || m_n!=nOwner)){
				M->nEnemy=nOwner;
				int price_dead=M->Die(power);
				if(price_dead){
					if(nOwner<=NUM_OPPONENT && m_n!=nOwner){
						Man[nOwner]->Cash(price_dead);
						if(GAME_TYPE==COMMAND_GAME){
							if(m_n>NUM_OPPONENT) Man[nOwner]->nKill++;
							else Man[nOwner]->nKill--;
						}else if(m_n<=NUM_OPPONENT){
							Man[nOwner]->nKill++;
							KILL[Man[nOwner]->number]++;
						}
					}
				}M->mv_angle=Direction(*M); M->mv_speed=power>>1;
			}
		}
	InSand();
}

int TNExpl::Move(){
	x+=COS(angle)>>4; y+=SIN(angle)>>4;
	x_obj=x; y_obj=y; r_obj=radius; pow_obj=power; t_obj=type;
	sz_obj=size; own_obj=nOwner; fz_obj=faze; AddObj(new TExpl);
	if(length<3) power++; if(size>5000) size-=1300;
	if(--length<1) return DELETE;
	register char w=world[x>>6][y>>6];
	if(w){ if(w<=22 || --power<=0) return DELETE; }
	return OK;
}

int TLiqExpl::Move(){
	if(world[x>>6][y>>6]) return DELETE;
	x_obj=x; y_obj=y; r_obj=radius; pow_obj=power; t_obj=type;
	sz_obj=size; own_obj=nOwner; fz_obj=faze;
	AddObj(new TExpl);
	int dx=COS(angle)>>4,dy=SIN(angle)>>4,ang_1=angle;;
	char &w=world[(x+dx)>>6][(y+dy)>>6];
	if(w){
		if(w>=23 && w<=28){ w-=rand()&3; if((w-=2)<=22) w=0; }
		if(rand()&1){ if((angle+=ANGLE_90)>=ANGLE_360) angle-=ANGLE_360; }
		else{ if((angle-=ANGLE_90)<0) angle+=ANGLE_360; }
		if(world[(x+(COS(angle)>>4))>>6][(y+(SIN(angle)>>4))>>6]){
			if((angle-=ANGLE_180)<0) angle+=ANGLE_360;
			if(world[(x+(COS(angle)>>4))>>6][(y+(SIN(angle)>>4))>>6])
				if((angle=(ang_1-ANGLE_180))<0) angle+=ANGLE_360;
		}
	}else{ x+=dx; y+=dy; }
	if(--length<1) return DELETE;
	if(size>5000) size-=1200;
	return OK;
}

int TPul::Burst(){
	x_obj=x; y_obj=y; r_obj=30; pow_obj=pow; sz_obj=2500; t_obj=EXPL_TEL;
	own_obj=nOwner; fz_obj=2; AddObj(new TExpl); return DELETE;
}

int TNap::Burst(){
	x_obj=x; y_obj=y; r_obj=45; pow_obj=POW_NAPALM; t_obj=EXPL_INV;
	sz_obj=1000; own_obj=nOwner; fz_obj=2; AddObj(new TExpl); return OK;
}

int TMis::Burst(){
	x_obj=x; y_obj=y; fz_obj=3; t_obj=0;
	sz_obj=14000; own_obj=nOwner;
	n_obj=5; pow_obj=POW_MIS; r_obj=65;
	ang_obj=angle; AddObj(new TNExpl);
	ang_obj=dAngle(ANGLE_120); AddObj(new TNExpl);
	ang_obj=dAngle(-ANGLE_120); AddObj(new TNExpl);
	return DELETE;
}

int TTaz::Burst(){
	x_obj=x; y_obj=y; sz_obj=13000;
	own_obj=nOwner; fz_obj=5; pow_obj=POW_TAZER; t_obj=EXPL_NMG;
	n_obj=9; r_obj=60;
	ang_obj=angle; AddObj(new TNExpl);
	ang_obj=dAngle(ANGLE_45); AddObj(new TNExpl);
	ang_obj=dAngle(-ANGLE_45); AddObj(new TNExpl);
	n_obj=5;
	ang_obj=dAngle(ANGLE_90); AddObj(new TNExpl);
	ang_obj=dAngle(-ANGLE_90); AddObj(new TNExpl);
	wait=2; return OK;
}

int TMav::Burst(){
	x_obj=x; y_obj=y; sz_obj=15000; own_obj=nOwner;
	r_obj=80; pow_obj=POW_MAV; fz_obj=4; t_obj=0; n_obj=8;
	ang_obj=dAngle(-ANGLE_30); AddObj(new TNExpl);
	ang_obj=dAngle(ANGLE_30); AddObj(new TNExpl);
	ang_obj=dAngle(-ANGLE_90); AddObj(new TNExpl);
	ang_obj=dAngle(ANGLE_90); AddObj(new TNExpl); n_obj=6;
	ang_obj=dAngle(-ANGLE_120); AddObj(new TNExpl);
	ang_obj=dAngle(ANGLE_120); AddObj(new TNExpl);
	int xw=(x>>6),yw=(y>>6);
	char w=world[xw][yw];
	if(w>0 && (w<WALL_SAND || w>=128)){
		xw--; yw--; // удалить 3x3
		for(int i=0; i<3; i++,xw++,yw-=3)
			for(int j=0; j<3; j++,yw++)
				if(xw>0 && yw>0 && xw<(WORLD_COL-1) && yw<(WORLD_ROW-1))
					world[xw][yw]=0;
		return DELETE;
	}return OK;
}

int TBarrelObl::Burst(){
	own_obj=nOwner; t_obj=0; r_obj=90;
	x_obj=x; y_obj=y; fz_obj=5; pow_obj=POW_OBL+(rand()&31);
	sz_obj=7500+(rand()&4096);
	AddObj(new TExpl);
	return DELETE;
}