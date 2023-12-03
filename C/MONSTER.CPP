#include "c\wea.h"
#include "c\monster.h"
#include "c\debug.h"
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>

// TMonster::TMonster():TBeing(){
// TIncubator::TIncubator():TObj(){
// void TMonster::Save(int hf){
// void TIncubator::Save(int hf){
// int TMonster::Die(int power){
// void TMonster::Sprite(int ang){
// void TIncubator::Sprite(int ang){
// int TMonster::ManVis(){
// void TMonster::Fire(){
// void TMonster::ChangeCnd(){
// void TMonster::GetNearMan(){
// void TMonster::BeginTurn(int t1,int t2){
// void TMonster::TurnToMan(){
// int TMonster::Move(){
// void TMonster::Step(){
// int TMonster::Give(int what,int a,int){
// int TIncubator::Move(){

const char _COND[MONST_TYPES][32]={
	{0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2}, // ящ
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2}, // во
	{0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,3,3,3}, // кл
	{0,0,0,0,0,0,0,0,0,0,0,1,1,1,4,4,4,4,4,4,4,4,4,2,2,2,2,2,2,2,2,2}, // дг
	{0,0,0,1,1,1,1,1,1,1,1,1,1,1,4,4,4,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2}, // зм
	{0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2}, // нс
	{0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3}, // зд
	{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2}  // гр
};
//        выносливость ( 128 == 100% )      ящ во кл дг зм нс зд гр
const int mons_health[MONST_TYPES]       = { 7, 8, 9,10,12,15,13,20};
const int mons_price[MONST_TYPES]        = {30,30,40,50,60,70,80,100};
const int mons_cnd_cnt_rand[MONST_TYPES] = {70,80,60,80,70,80,90,99};
const int mons_cnd_cnt_plus[MONST_TYPES] = {60,30,50,50,50,40,60,70};
const int mons_sp_rnd[MONST_TYPES]       = { 5,10, 3, 4, 5, 7,12, 4};
const int mons_sp_chs[MONST_TYPES]       = { 7,11, 7, 6, 4, 8, 6, 5};
const int mons_sp_rag[MONST_TYPES]       = { 5, 9, 8, 9, 7, 9, 6, 3};
const int mons_turn_chs[MONST_TYPES]     = { 7,11,10, 8, 7, 4, 5,11};
const int mons_turn_rag[MONST_TYPES]     = { 9,16,14, 8,10, 8, 9, 5};
const int mons_byte_pw1[MONST_TYPES]     = {75,50,65,80,90,85,80,99};
const int mons_byte_pw2[MONST_TYPES]     = {55,65,60,65,45,55,70,80};
const int mons_byte_pw3[MONST_TYPES]     = {25,40,30,30,30,20,35,60};
const int mons_fire_fz1[MONST_TYPES]     = {22,20,25,30,33,30,27,24};
const int mons_fire_fz2[MONST_TYPES]     = {18,13,10,28,20,22,20,15};

TMonster::TMonster():TBeing(){
	if(n_obj>=(MIN_MONSTER_TYPE+MONST_TYPES) || n_obj<MIN_MONSTER_TYPE)
		n_obj=(MIN_MONSTER_TYPE+MONST_TYPES);
	number=n_obj;
	type=n_obj-12;
	turn=0; fzStep=1+(rand()&7); fzFire=0; pain=cnd=0;
	count=(rand()&63)+100;
	NUM_MONS++;
	age=angle=0;
	x=x_obj; y=y_obj;
	health=MAX_HEALTH;
	if(!x || !y) RndTelep();
}

TIncubator::TIncubator():TObj(){
	type=t_obj; speed=n_obj; age=24500;
	NUM_INCUB++;
}

void TMonster::Save(int hf){
	char c=T_MONSTER; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(TMonster)-2);
}
void TIncubator::Save(int hf){
	char c=T_INCUBATOR; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(TIncubator)-2);
}

int TMonster::Die(int power){
	int tele_expl=(power>250);
	power<<=7;
	power/=mons_health[type];
	if(health>0){
		if((health-=power)<=0){
			if(tele_expl) health=1;
			else{
				health=0; fzDead=1; Bits(5);
				register int price_ret=mons_price[type];
				if(!NUM_MONS) price_ret+=100;
				return price_ret;
			}
		}
	}
	if(tele_expl){ RndTelep(); pain=50; }
	else pain=4;
	return 0;
}

void TMonster::Sprite(int ang){
	tex_show=1; mod_turn=angle-ang; tex_index=(number<<3);
	mod_lower=4000-(age>>3); mod_sc=r+58-(age>>9);
	if(fzDead){ tex_index+=(2+(fzDead>>2)); if(_life==0) tex_show=99; }
	else if(fzFire) tex_index+=(3+((fzFire>>3)&1));
	else if(speed) tex_index+=fzStep>>2;
}

void TIncubator::Sprite(int ang){
	mod_lower=0; tex_show=1; mod_turn=-ang;
	mod_sc=r; tex_index=67-(age>>12);
}

int TMonster::ManVis(){
	int X=x,Y=y;
	for(register int n=r2man>>6; n>1; n--){
		X+=COS(a2man)>>3; Y+=SIN(a2man)>>3;
		if(world[X>>6][Y>>6])
			return 0;
	}return 1;
}

void TMonster::Fire(){
	if(pain) return;
	speed=0;
	x_obj=x+(COS(angle)>>3); y_obj=y+(SIN(angle)>>3);
	r_obj=40; t_obj=EXPL_INV; own_obj=number;
	pow_obj=mons_byte_pw1[type]+(rand()&mons_byte_pw2[type]);
	if(cnd==CND_RAGE)
		pow_obj+=mons_byte_pw3[type]; //если монстр в ярости, кусает сильнее
	sz_obj=5000; fz_obj=2; AddObj(new TExpl);
}

void TMonster::ChangeCnd(){
	nEnemy=255;
	count=(rand()%mons_cnd_cnt_rand[type])+mons_cnd_cnt_plus[type];
	// При малом количестве монстров размножение усиливается
	int i_cnd=(rand()&31),i_dupl;
	cnd=_COND[type][i_cnd];
	if(NUM_MONS<3) i_dupl=25;
	else if(NUM_MONS<5) i_dupl=26;
	else if(NUM_MONS<10) i_dupl=28;
	else if(NUM_MONS<15) i_dupl=29;
	else if(NUM_MONS<20) i_dupl=30;
	else i_dupl=9999;
	i_dupl-=NUM_OPPONENT; // чем > игроков, тем быстрее размножение
	if(age>20000) i_dupl--; // старые монстры размножаются быстрее
	if(age>15000) i_dupl--;
	if(age>10000) i_dupl--;
	if(i_cnd>i_dupl && age>4000) cnd=CND_DUPL;
	if(cnd==CND_DUPL) count+=250;
}

void TMonster::GetNearMan(){
	register int j=0;
	int r_min,r_invis;
	n_man=99;
	if(cnd==CND_RAGE){
		if(nEnemy<=NUM_OPPONENT || Man[nEnemy]->fzDead){
			r_min=Man[nEnemy]->R(*this);
			n_man=nEnemy;
		}
	}else{
		r_min=r_invis=32000;
		for(j=0; j<=NUM_OPPONENT; j++){
			if(Man[j]->health){
				TMan &M=*Man[j];
				r2man=R(M); a2man=Direction(M);
				if(r_min>r2man && ManVis()){
					if(!M.invis){ n_man=j; man_hol=1; r_min=r2man; }
					else r_invis=r2man;
				}
			}
			if(Hol[j]->vis){
				THol &H=*Hol[j];
				r2man=R(H); a2man=Direction(H);
				if(r_min>r2man && ManVis()){
					n_man=j; man_hol=0; r_min=r2man;
				}
			}
		}
	}
	if(n_man<=NUM_OPPONENT){
		r2man=r_min;
		if(man_hol) a2man=Man[n_man]->GetAngle(*this);
		else a2man=Hol[n_man]->GetAngle(*this);
	}else{
		if(n_man>NUM_OPPONENT && (cnd==CND_CHASE || cnd==CND_WAIT))
			cnd=CND_RAND;
		r2man=r_invis;
	}
}

void TMonster::BeginTurn(int t1,int t2){
	if(turn==0){
		turn=t1+(rand()%t2);
		if(rand()&1) turn=-turn;
	}
}

void TMonster::TurnToMan(){
	register int t=mons_turn_chs[type];
	if(a2man>0) Turn(a2man<t?a2man:t);
	else if(a2man<0) Turn(a2man>-t?a2man:-t);
}

int TMonster::Move(){
	x_obj=x; y_obj=y;
	if(fzDead>6){
		ang_obj=angle; n_obj=number; AddObj(new TCorpse);
		NUM_MONS--;
		return DELETE;
	}
	if(fzDead>0) fzDead++;
	else if(age<30000){ if(((age++)&3)==0) health++; }

	if(pain){ Blood((rand()&7)+7,(rand()&7)+10); pain--; }
	if(++fzStep>10) fzStep=1;
	if(fzFire) fzFire--;
	GetNearMan();
	int x1,y1;
	register int t;
	switch(cnd){
		case CND_RAND:
			x1=x+(COS(angle)>>3); y1=y+(SIN(angle)>>3);
			if(!(rand()&7)){
				if(world[x1>>6][y1>>6]){ turn=0; BeginTurn(45,45); }
				else BeginTurn(20,10);
			}
			speed=mons_sp_rnd[type];
			break;
		case CND_TELE:
			if(count>7){
				BeginTurn(20,20); speed=0;
			}else if(count==7){
				r_obj=100; pow_obj=15; t_obj=EXPL_NMG; sz_obj=12222; fz_obj=8;
				own_obj=number;
				AddObj(new TExpl); RndTelep(); speed=mons_sp_chs[type];
			}
			break;
		case CND_DUPL:
			if(count>25){
				BeginTurn(10,10); speed=(rand()&3)-1;
			}else if(count==25){
				t_obj=((type+1)&7); n_obj=11-type+(rand()&15)+(age>>11);
				AddObj(new TIncubator);
			}else{
				if(world[x1>>6][y1>>6]){ turn=0; BeginTurn(45,45); }
				else BeginTurn(10,10);
				speed=mons_sp_chs[type];
			}
			break;
		case CND_SLIP:
			if(!(rand()&7)){ turn=0; BeginTurn(50,25); }
			TurnToMan();
			if(r2man>60) speed=mons_sp_chs[type]; else speed=-3;
			break;
		case CND_CHASE:
			turn=0; TurnToMan();
			if(r2man>60) speed=mons_sp_chs[type]; else speed=0;
//			FireChance=rand()&3;
			break;
		case CND_WAIT:
			BeginTurn(0,100);
			speed=0;
//			FireChance=rand()%2;
			break;
		case CND_RAGE:
			turn=0; t=mons_turn_rag[type];
			if(a2man>0) Turn(a2man<t?a2man:t);
			else if(a2man<0) Turn(a2man>-t?a2man:-t);
			if(r2man>60) speed=mons_sp_rag[type]; else speed=0;
//			FireChance=rand()%3;
			break;
	}

	Step();

	if(cnd!=CND_RAGE && nEnemy<=NUM_OPPONENT && Man[nEnemy]->fzDead==0 && !(rand()&31)){
		cnd=CND_RAGE; count=(rand()%mons_cnd_cnt_rand[type])+mons_cnd_cnt_plus[type];
	}

	if(r2man<70 && !fzFire){
		turn=0; fzFire=mons_fire_fz1[type];
	}
	if(fzFire==mons_fire_fz2[type]) Fire();

	if(turn>0){ turn--; Turn(3); }
	else if(turn<0){ turn++; Turn(-3); }

	if(cnd==CND_RAND && n_man<=NUM_OPPONENT)
		cnd=CND_CHASE;
	if(!(--count))
		ChangeCnd();
	int x_index=(x+(COS(angle)>>4))>>6,y_index=(y+(SIN(angle)>>4))>>6;
	char wrld=world[x_index][y_index];
	if(wrld>=128){
		wrld-=128;
		Telep(((int)tel_w_x[wrld]<<6)+32,((int)tel_w_y[wrld]<<6)+32);
	}else if(world[x_index][y_index]>22 && !fzFire){
		turn=0; fzFire=mons_fire_fz1[type];
	}
#if IS_DEBUG
	extern char *NAME_MONS[];
	Msg(NAME_MONS[type]); Msg(" cnd="); Msg(cnd);
	Msg(" health="); Msg(health);
	Msg(" age="); Msg(age); Msg("\xFF");
#endif
	return OK;
}

void TMonster::Step(){
	int into=(world[x>>6][y>>6]>1);
	if(!into && cnd==CND_SLIP && count<5)
		ChangeCnd();
	for(int i=0; i<=NUM_OBJ; i++)
		if(R(*Obj[i])<35){
			if(Obj[i]->Take(*this)==TAKE) DelObj(i);
//			else if((cnd==CND_SLIP) && (Obj[i]->Its()==ITS_MAN)) into=1;
		}
	if(cnd==CND_SLIP){
		SlipStep();
		if(count<2 && into) count=2;
	}else{
		if(into) Die(8),nEnemy=255;
		TBeing::Step();
	}
}

int TMonster::Give(int what,int a,int){
	switch(what){
	case 0:
		Destroy(); ITEMS_LEFT--; return TAKE;
	case 5:
		health+=MAX_HEALTH; ITEMS_LEFT--; return TAKE;
	case 99:
		GOLD_LEFT-=a; return TAKE;
	case 100:
		if(health>=MAX_HEALTH) return NOTAKE;
		if((health+=800)>MAX_HEALTH) health=MAX_HEALTH;
		ITEMS_LEFT--; return TAKE;
	}return NOTAKE;
}

int TIncubator::Move(){
	if(world[x>>6][y>>6]){
		NUM_INCUB--;
		return DELETE;
	}
#if IS_DEBUG
	Msg("I("); Msg(type); Msg(") age="); Msg(age); Msg(" speed="); Msg(speed); Msg("\xFF");
#endif
	int no_on=1; // никто не стоит на яйце?
	for(register int i=0; i<=NUM_OBJ; i++)
		if(R(*Obj[i])<50){
			if(Obj[i]->Its()==ITS_EXPL){
				TExpl *Ex=(TExpl*)Obj[i];
				own_obj=Ex->nOwner; t_obj=0; r_obj=50;
				for(register int j=0; j<5; j++){
					x_obj=x+(rand()&127)-63; y_obj=y+(rand()&127)-63;
					fz_obj=5+(rand()&3); pow_obj=POW_INCUBATOR+(rand()&31);
					sz_obj=8000+(rand()&4096);
					AddObj(new TExpl);
				}
				NUM_INCUB--;
				return DELETE;
			}else if(Obj[i]->Its()==ITS_MAN){
				TBeing *M=(TBeing*)Obj[i];
					if(M->number<=NUM_OPPONENT){
					M->nEnemy=255;
					M->Die(DIE_ON_INCUB);
				}
				no_on=0;
			}
		}
	if(age<speed){
		if(no_on){
			x_obj=x; y_obj=y;
			n_obj=MIN_MONSTER_TYPE+type;
			AddObj(new TMonster);
			fz_obj=6; pow_obj=1; sz_obj=12000;
			AddObj(new TExpl);
			NUM_INCUB--;
			return DELETE;
		}else	age+=1000;
	}else{
		age-=speed;
	}
	return OK;
}
