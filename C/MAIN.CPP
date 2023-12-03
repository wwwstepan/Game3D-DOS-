#include <mem.h>
#include <stdlib.h>
#include "c\wea.h"
#include "c\obj.h"
#include "c\engine.h"

// void main(){
// void TManInf::New(int n){
// void TManInf::MakeMan(){
// void TMan::GetInfo(TManInf &M){
// void TMan::FindWeap(){
// void TManInf::GetInfo(){
// void TManInf::Sell(){
// void TManInf::Give(){
// void TManInf::Buy0(){
// void TManInf::Buy1(){
// void TManInf::Buy2(){
// void TMan::CalcMaxSpeed(){
// void TMan::Inp0(){
// void TMan::Inp1(){
// void TMan::Inp2(){
// void TMan::DropWeap(){
// int TMan::Give(int what,int a,int b){
// void TMan::DrawMap(){
// void TMan::DrawMap(int i,int j){
// void TMan::Resurrect(){
// void TMan::StateInf(){

/********************************/
void main(){
	TOMArEngine *E=new TOMArEngine;
	E->Run();
	delete E;
}
/********************************/

extern unsigned _stklen=10000U;
TOMArFooter FTR;
RGB RGB_COLOR[256];
TKeyboard KB;
//--------------------------------------------------------------
volatile int raw_key;
volatile char key[128];
char *GrArea=(char*)0xA0000000L,*GrBuff,*GrPtr,*LightLev,*CharPtr;
//--------------------------------------------------------------
int NUM_OBJ,NUM_OPPONENT;
volatile UNS TIME_LEFT;
//--------------------------------------------------------------
char world[WORLD_COL][WORLD_ROW];
char tel_w_x[128],tel_w_y[128];
//--------------------------------------------------------------
float *tan_table,*inv_tan_table,*y_step,*x_step;
float *cos_table,*inv_cos_table,*inv_sin_table;
int *SIN_TABLE,Dist[ANGLE_60];

int tab_angle[128][128],tab_distance[64][64];

unsigned long tex_dy,tex_index2;
char *pWall,*pBurst,*pFon6,*pFacet,*_n_,*tex_row,*tex_ptr;
unsigned *Clock=(unsigned far*)0x0000046C;
volatile unsigned tic=0;

char *RES_FILE_NAME="omar.res";
char SAVE_FILE_NAME[13]="quick.sav";
char *LAST_FILE_NAME="last.sav";
char *PREV_FILE_NAME="prev.sav";
char *EXIT_FILE_SAVE="exit.sav";

char NAME[3][16]={"Коля DooM","Рома OMAr","Митя Quake"};
char *NAME_MONS[]={
	"\'ящер\'","\'воробей\'","\'клоп\'","\'диггер\'",
	"\'змей\'","\'носорог\'","\'злой дух\'","\'гризли\'",
	"M9","M10","M11","M12","M13","M14","M15","M16",
};
char *STR_DTA[]={
	"Ошибка в файле опций.","\x7 \x9 \xB \xD  Ctrl Shift Alt",
	"МЫШЬ","G R D F   S     W    Q",
	"\nПоврежден файл omar.exe\nВозможно, это действие вируса или неграмотного хакера\n",
	"Поврежден файл ресурсов","Не хватает оперативной памяти",
	"Ошибка при распределении памяти","Неверный формат файла *.sav"
};
char *INFO[]={
	"Ружье","Подобрал пулемёт","Электрошок!","Это же\xFFТАЗЕР","Наводящаяся\xFFракета",
	"Маверик","Мина","Бульдозер","Голограма","Переносной\xFFтелепорт!",
	"","","","","","","","","Кристал","Рентген","Нашел броню!",
	"подобрал золото","25% жизни!","Тебя убил","Ты погиб","?","It's КОДЫ!",
	"Игра сохранена","Игра введена","НЕВИДИМОСТЬ","100% жизни !!!",
	"ТЫ\xFFЗАМОЧИЛ\xFFВРАГА!","Чудак!\xFFТы убил\xFFсебя сам."
};
char *WEAPS_NAME[]={
	"","Ружье","Пулемёт","Эл.шок","Тазер","Ракета","Маверик","Мина",
	"Бульдозер","Голограма","Телепорт","Броня","Рентген","Радар","Др.уровень","*"
};
char *GAMETYPES[]={"без монстров","каждый сам за себя","против монстров"};

char STR_LEV_PASS_LEFT[6]="  /  ",STR_GOLD_LEFT[6]="$    ",
	STR_TIME_LEFT[6]="  :  ",STR_MONS_LEFT[7]="   Mon";

int GAME_IS_GO=1,GOLD_LEFT,ITEMS_LEFT,LEVEL_TYPE,NUM_MONS,NUM_INCUB,GAME_TYPE=0;
int IS_MONSTERS;
int tex_x,tex_top,tex_end,tex_index,tex_n,tex_column,tex_size,tex_scale,
	tex_shad,tex_show,mod_lower,tri_scr_x_min,mod_turn,mod_r,mod_sc;
int sw_buff=1,items_turn_ang=0;
UNS light_seg;
int x_obj,y_obj,ang_obj,t_obj,n_obj,own_obj;
int h_obj,pow_obj,fz_obj,r_obj,sz_obj;
int N_MAN_DRAW;

UNS CUR_LEV=0,PREV_LEV=0,PREV2_LEV=0,PREV3_LEV=0,NEXT_LEV=0;

TMouse MOUSE;
TMan *Man[3];
TArm *Arm[3];
THol *Hol[3];
TArm *hArm[3];
TObj *Obj[MAX_NUM_OBJ+1];//
////////////////////////кир,руж,пул,элш,таз,рок,мав,мин,бул,гол,тел,бро,рен,рад,зам
const int PLUS_AMMO[]  = {0, 16,221, 21, 11,  8,  5,  3, 18,141,  2};         //ур.
const char NUM_AMMO[]  = {0, 10, 80, 10,  5,  5,  1,  1,  3, 60,  1};
const char AMMO_PACK[] = {0,  1, 10, 10,  1,  1,  1,  1,  1, 60,  1, 10, 10,  1,  1};
int WEAP_PRICE_BASE[]  = {0,  1, 15,220, 20, 20, 70, 60,  5, 40, 80,200, 20, 50, 10};
int WEAP_PRICE[]       = {0,  1, 15,220, 20, 20, 70, 60,  5, 40, 80,200, 20, 50, 10};
const int  MAX_AMMO[]  = {1,200,999,999,100,100, 50, 10,100,999, 10, 99, 99,999,  0};

const int HSTEP[11]={0,1,2,1,1,0,-1,-2,-1,-1,0};
const int FZ_WEAP[]={
	KIR_FZ,GUN_FZ,PLM_FZ,SHK_FZ,TAZ_FZ,
	ROC_FZ,MAV_FZ,MIN_FZ,BLD_FZ,HOL_FZ,TEL_FZ
};
const int MAN_SCR_X[3]={5,110,215};

int LEVELS_PASS=1;
char LIC_WEA[]={4,0,1,2,0,1,2,3,3,3,3,3,3,3,3};
char CONTROL[3]={0,1,2};
int MAN_PRICE[3]={200,200,200};
int MAN_HLTH[3]={64,64,64};
char IX_SCR[3]={0,2,1};
char KILLED[3]={0,0,0}; // сколько раз убили игрока
char KILL[3]={0,0,0};   // сколько человек убил игрок

void (TMan::*ManInput[3])()={TMan::Inp0,TMan::Inp1,TMan::Inp2};
void (TManInf::*ManInfBuy[3])()={TManInf::Buy0,TManInf::Buy1,TManInf::Buy2};

void TManInf::New(int n){
	setmem(this,sizeof(TManInf),0);
	health=MAX_HEALTH; number=n; ammo[0]=1; ammo[1]=20;
}

void TManInf::MakeMan(){
	Man[number]->number=number;
	Man[number]->slip=slip; Man[number]->rent=rent; Man[number]->invis=invis;
	Man[number]->radar=radar; Man[number]->health=health;
	Man[number]->armor=armor; Man[number]->cash=cash; Man[number]->nKill=nKill;
	Man[number]->doh=doh;
	for(int i=0; i<NUM_WEAP; i++)
		Man[number]->ammo[i]=ammo[i];
}

void TMan::GetInfo(TManInf &M){
	setmem(&M,sizeof(TManInf),0);
	M.health=health<MAX_HEALTH?MAX_HEALTH:health;
	M.slip=slip; M.rent=rent; M.armor=armor; M.invis=invis;
	M.cash=cash; M.isOk=0; M.nKill=nKill; M.number=number;
	M.radar=0; M.doh=doh;
	for(int i=0; i<NUM_WEAP; i++)
		M.ammo[i]=ammo[i];
}

void TMan::FindWeap(){
	for(register int i=1; i<NUM_WEAP && LIC_WEA[i]!=number && LIC_WEA[i]!=3; ++i);
	if(i>=NUM_WEAP) i=1;
	ammo[i]=NUM_AMMO[i]; nWeap=i;
}

void TManInf::GetInfo(){
	int X=MAN_SCR_X[IX_SCR[number]];
	if(nBuyWea<SHOP_EXIT){
		String(WEAPS_NAME[nBuyWea],X+1,64);
		String("$",X+59,64);
		String(WEAP_PRICE[nBuyWea],X+65,64);
		if(nBuyWea<SHOP_RADAR){
			String("\\",X+83,64); String(AMMO_PACK[nBuyWea],X+89,64);
		}
		String("$",X+23,73); String(cash,X+28,73);
		if(nBuyWea==SHOP_ARMOR){
			if(armor){ ClrCharAt(6,X+57,73); String(armor>>7,X+63,73); }
		}else if(nBuyWea==SHOP_RENTGEN){
			if(rent){ ClrCharAt(5,X+57,73); String(rent>>5,X+63,73); }
		}else if(nBuyWea==SHOP_RADAR){
			String(radar?"есть":"нет",X+57,73);
		}else if(nBuyWea==SHOP_LEVEL){
		}else{
			if(ammo[nBuyWea]){
				ClrCharAt(1,X+57,73);
				String(ammo[nBuyWea],X+63,73);
			}
		}
	}else{ String("Я готов!",X+23,64); if(isOk) String("Ok",X+45,73); }
}

const float SELL_PAY=.8;

void TManInf::Sell(){
	float wp;
	if(nBuyWea>=SHOP_EXIT) isOk=1;
	else if(nBuyWea<NUM_WEAP){
		wp=SELL_PAY*WEAP_PRICE[nBuyWea]+.5;
		if(ammo[nBuyWea]>=AMMO_PACK[nBuyWea]){
			CashSell(wp);
			if((ammo[nBuyWea]-=AMMO_PACK[nBuyWea])<1)
				ammo[nBuyWea]=0;
		}else{
			wp*=ammo[nBuyWea];
			wp/=AMMO_PACK[nBuyWea];
			CashSell(wp);
			ammo[nBuyWea]=0;
		}
	}else if(nBuyWea==SHOP_RENTGEN){
		wp=SELL_PAY*WEAP_PRICE[nBuyWea]+.5;
		wp*=(rent>>5);
		wp/=AMMO_PACK[nBuyWea];
		CashSell(wp);
		rent=0;
	}
}

void TManInf::Give(){
	extern char Change_level;
	if(nBuyWea>=SHOP_EXIT){ isOk=1; return; }
	int wp=WEAP_PRICE[nBuyWea],pack=AMMO_PACK[nBuyWea];
	if(cash>0 && cash<wp){
		pack*=((float)cash/(float)wp);
		wp=cash;
	}
	if(cash>=wp && pack>0){
		if(nBuyWea==SHOP_ARMOR){
			if(armor<MAX_ARMOR){
				cash-=wp;
				if((armor+=(pack<<7))>MAX_ARMOR)
					armor=MAX_ARMOR;
			}
		}else if(nBuyWea==SHOP_RENTGEN){
			if(rent<MAX_RENT){
				cash-=wp;
				if((rent+=(pack<<5))>MAX_RENT)
					rent=MAX_RENT;
			}
		}else if(nBuyWea==SHOP_RADAR){
			if(!radar) radar=1,cash-=wp;
		}else if(nBuyWea==SHOP_LEVEL){
			if(Change_level==0)
				cash-=wp,Change_level=15;
		}else if(ammo[nBuyWea]<MAX_AMMO[nBuyWea]){
			cash-=wp;
			if((ammo[nBuyWea]+=pack)>MAX_AMMO[nBuyWea])
				ammo[nBuyWea]=MAX_AMMO[nBuyWea];
		}
	}
}

void TManInf::Buy0(){
	for(int i=0; i<NUM_WEAP && LIC_WEA[i]!=number && LIC_WEA[i]!=3; i++);
	if(key[iUP]){
		do nBuyWea--;
		while(LIC_WEA[nBuyWea]!=number && LIC_WEA[nBuyWea]!=3);
		if(nBuyWea<i) nBuyWea=i;
	}
	if(key[iDOWN]){
		do nBuyWea++;
		while(LIC_WEA[nBuyWea]!=number && LIC_WEA[nBuyWea]!=3);
		if(nBuyWea>SHOP_EXIT) nBuyWea=SHOP_EXIT;
	}
	if(nBuyWea<SHOP_EXIT) isOk=0;
	if(key[iCTRL]) Give(); else if(key[iRSHIFT]) Sell();
}

void TManInf::Buy1(){
	for(int i=0; i<NUM_WEAP && LIC_WEA[i]!=number && LIC_WEA[i]!=3; i++);
	MOUSE.Rel(); MOUSE.Get();
	int dy=(MOUSE._y);
	if(dy<-30){
		do nBuyWea--;
		while(LIC_WEA[nBuyWea]!=number && LIC_WEA[nBuyWea]!=3);
		if(nBuyWea<i) nBuyWea=i;
	}
	if(dy>30){
		do nBuyWea++;
		while(LIC_WEA[nBuyWea]!=number && LIC_WEA[nBuyWea]!=3);
		if(nBuyWea>SHOP_EXIT) nBuyWea=SHOP_EXIT;
	}
	if(nBuyWea<SHOP_EXIT) isOk=0;
	if(MOUSE.b==1) Give(); else if(MOUSE.b==2) Sell();
}

void TManInf::Buy2(){
	for(int i=0; i<NUM_WEAP && LIC_WEA[i]!=number && LIC_WEA[i]!=3; i++);
	if(key[iR]){
		do nBuyWea--;
		while(LIC_WEA[nBuyWea]!=number && LIC_WEA[nBuyWea]!=3);
		if(nBuyWea<i) nBuyWea=i;
	}
	if(key[iF]){
		do nBuyWea++;
		while(LIC_WEA[nBuyWea]!=number && LIC_WEA[nBuyWea]!=3);
		if(nBuyWea>SHOP_EXIT) nBuyWea=SHOP_EXIT;
	}
	if(nBuyWea<SHOP_EXIT) isOk=0;
	if(key[iS]) Give(); else if(key[iW]) Sell();
}

const UNS MAX_FIRE_TIME=16;
const int PAIN_SPEED=3; // 01 02 03 04 05 06 07 08 09 10 11 12
const int MAX_TURN_KB_[]= {30,29,31,30,29,28,28,28,27,27,27,27};
const int MAX_TURN_MOU_[]={28,27,29,28,27,27,27,26,26,26,26,26};
const int MAX_SPEED_[]=   {14,13,12,11,10,10, 9, 9, 8, 8, 7, 7};

int MAX_TURN_KB=0,MAX_TURN_MOU=0,MAX_SPEED=0;

void TMan::CalcMaxSpeed(){
	register int nw=0;
	for(register int i=2; i<NUM_WEAP; i++)
		if(ammo[i]>3)
			nw++;
	MAX_TURN_KB=MAX_TURN_KB_[nw];
	MAX_TURN_MOU=MAX_TURN_MOU_[nw];
	MAX_SPEED=MAX_SPEED_[nw];

	if(health<=MAX_HEALTH/7 || health>MAX_HEALTH){ MAX_SPEED++; MAX_TURN_KB--; MAX_TURN_MOU--; }
	if(health<=MAX_HEALTH/13){ MAX_SPEED++; MAX_TURN_KB--; MAX_TURN_MOU--; }
}

void TMan::Inp0(){
	UNS static fire_time=0;
	CalcMaxSpeed();
	if(_life>100) return;
	if(key[iALT] && mv_speed>=0){
		if(key[iLEFT]){ w_sp=0; mv_angle=dAngle(ANGLE_90); mv_speed=MAX_SPEED; }
		else if(key[iRIGHT]){ w_sp=0; mv_angle=dAngle(-ANGLE_90); mv_speed=MAX_SPEED; }
	}else{
		register int w=w_sp?4+(tic&3):1;
		if(key[iRIGHT] && key[iLEFT]) w_sp=0;
		else if(key[iRIGHT]){ if(w_sp>0) w_sp=0; w_sp-=w; }
		else if(key[iLEFT]){ if(w_sp<0) w_sp=0;  w_sp+=w; }
		else w_sp=0;
		if(w_sp<-MAX_TURN_KB) w_sp=-MAX_TURN_KB;
		else if(w_sp>MAX_TURN_KB) w_sp=MAX_TURN_KB;
	}
	Turn(w_sp);
	if(_life==0){ if(strcnt<3){ Resurrect(); }return; }
	if(_life==2) _life=1;
	if(key[iRSHIFT]) ChangeWeap(),_life=2;
	if(key[iCTRL]){
		if(fire_time<MAX_FIRE_TIME) fire_time++;
		_life=2; if(!fzWeap && !pain && ammo[nWeap]) fzWeap=FZ_WEAP[nWeap];
	}else{
		if(fire_time>=MAX_FIRE_TIME) DropWeap();
		if(!ammo[nWeap]){ ammo[nWeap]=0; ChangeWeap(); }
		fire_time=0;
	}
	if(key[iUP]) speed++; if(key[iDOWN]) speed--;
	if(pain){
		if(speed>PAIN_SPEED) speed=PAIN_SPEED;
		else if(speed<-PAIN_SPEED) speed=-PAIN_SPEED;
	}
	if(!key[iUP] && !key[iDOWN] && speed!=0) speed+=speed>0?-1:1;
	if(speed>MAX_SPEED) speed=MAX_SPEED;
	else if(speed<-MAX_SPEED) speed=-MAX_SPEED;
}
void TMan::Inp1(){
	UNS static fire_time=0;
	UNS static rb_time=0;
	CalcMaxSpeed();
	if(_life>100) return;
	MOUSE.Rel(); MOUSE.Get();
	int dx=-(MOUSE._x>>2),dy=-(MOUSE._y>>2);
	if(dx>MAX_TURN_MOU) dx=MAX_TURN_MOU;
	else if(dx<-MAX_TURN_MOU) dx=-MAX_TURN_MOU;
	w_sp=dx; Turn(w_sp);
	if(_life==0){ if(strcnt<3){ Resurrect(); }return; }
	if(dy>MAX_SPEED) dy=MAX_SPEED;
	else if(dy<-MAX_SPEED) dy=-MAX_SPEED;
	speed=dy;
	if(pain){
		if(speed>PAIN_SPEED) speed=PAIN_SPEED;
		else if(speed<-PAIN_SPEED) speed=-PAIN_SPEED;
	}
	if(_life==2) _life=1;
	if(MOUSE.b&'\xFE'){
		if(rb_time<8) rb_time++;
		else speed=MAX_SPEED;
	}else{
		if(rb_time>0 && rb_time<8) ChangeWeap(),_life=2;
		rb_time=0;
	}
	if(MOUSE.b&'\xFD'){
		if(fire_time<MAX_FIRE_TIME) fire_time++;
		_life=2; if(!fzWeap && !pain && ammo[nWeap]) fzWeap=FZ_WEAP[nWeap];
	}else{
		if(fire_time>=MAX_FIRE_TIME) DropWeap();
		if(!ammo[nWeap]){ ammo[nWeap]=0; ChangeWeap(); }
		fire_time=0;
	}
}
void TMan::Inp2(){
	UNS static fire_time=0;
	CalcMaxSpeed();
	if(_life>100) return;
	if(key[iQ] && mv_speed>=0){
		if(key[iD]){ w_sp=0; mv_angle=dAngle(ANGLE_90); mv_speed=MAX_SPEED; }
		else if(key[iG]){ w_sp=0; mv_angle=dAngle(-ANGLE_90); mv_speed=MAX_SPEED; }
	}else{
		register int w=w_sp?4+(tic&3):1;
		if(key[iD] && key[iG]) w_sp=0;
		else if(key[iG]){ if(w_sp>0) w_sp=0;  w_sp-=w; }
		else if(key[iD]){ if(w_sp<0) w_sp=0;  w_sp+=w; }
		else w_sp=0;
		if(w_sp<-MAX_TURN_KB) w_sp=-MAX_TURN_KB;
		else if(w_sp>MAX_TURN_KB) w_sp=MAX_TURN_KB;
	}
	Turn(w_sp);
	if(_life==0){ if(strcnt<3){ Resurrect(); }return; }
	if(_life==2) _life=1;
	if(key[iW]) ChangeWeap(),_life=2;
	if(key[iS]){
		if(fire_time<MAX_FIRE_TIME) fire_time++;
		_life=2; if(!fzWeap && !pain && ammo[nWeap]) fzWeap=FZ_WEAP[nWeap];
	}else{
		if(fire_time>=MAX_FIRE_TIME) DropWeap();
		if(!ammo[nWeap]){ ammo[nWeap]=0; ChangeWeap(); }
		fire_time=0;
	}
	if(key[iR]) speed++; if(key[iF]) speed--;
	if(pain){
		if(speed>PAIN_SPEED) speed=PAIN_SPEED;
		else if(speed<-PAIN_SPEED) speed=-PAIN_SPEED;
	}
	if(!key[iR] && !key[iF] && speed!=0) speed+=speed>0?-1:1;
	if(speed>MAX_SPEED) speed=MAX_SPEED;
	else if(speed<-MAX_SPEED) speed=-MAX_SPEED;
}

void TMan::DropWeap(){
	if(nWeap && ammo[nWeap]){
		x_obj=x+(COS(angle)>>4); y_obj=y+(SIN(angle)>>4);
		r_obj=ammo[nWeap]; ammo[nWeap]=0; t_obj=nWeap; nWeap=0;
		ang_obj=angle; h_obj=-9000; n_obj=20;
		AddObj(new TWeap); ITEMS_LEFT++;
	}
}

int TMan::Give(int what,int a,int b){
	switch(what){
	case 0: // оружие
		if(health<1) return NOTAKE;
		if(ammo[b]<MAX_AMMO[b]){
			if((ammo[b]+=a)>MAX_AMMO[b]) ammo[b]=MAX_AMMO[b]; Info(--b);
		}else return NOTAKE;
		ITEMS_LEFT--; return TAKE;
	case 1: // кристалл
		if(slip>=MAX_SLIP) return NOTAKE; if((slip+=160)>MAX_SLIP) slip=MAX_SLIP;
		Info(18); ITEMS_LEFT--; return TAKE;
	case 2: // рентген
		if(rent>=MAX_RENT) return NOTAKE; if((rent+=640)>MAX_RENT) rent=MAX_RENT;
		Info(19); ITEMS_LEFT--; return TAKE;
	case 3: // броня
		if(armor>=MAX_ARMOR) return NOTAKE;
		if((armor+=1280)>MAX_ARMOR)
			armor=MAX_ARMOR;
		Info(20); ITEMS_LEFT--; return TAKE;
	case 4: // невидимость
		if(invis>=25344) return NOTAKE;
		if(invis<(25344-5120)) invis+=5120; else invis=25344;
		Info(29); ITEMS_LEFT--; return TAKE;
	case 5: // 100%
		health+=MAX_HEALTH; Info(30); ITEMS_LEFT--; return TAKE;
	case 99: // золото
		GOLD_LEFT-=a; Cash(a); Info(21); return TAKE;
	case 100: // шприц
		//if(health>=MAX_HEALTH) return NOTAKE;
		//if((health+=800)>MAX_HEALTH) health=MAX_HEALTH;
		health+=800; Info(22); ITEMS_LEFT--; return TAKE;
	}return NOTAKE;
}

void TMan::DrawMap(){
	for(int i=0; i<WORLD_COL; i++)
		for(int j=0; j<WORLD_ROW; j++)
			DrawMap(i,j);
	int rad;
	if(radar){
		rad=tic&63;
		if(rad<15)
			for(i=0; i<=NUM_OBJ; i++)
				if(Obj[i]->Its()==ITS_MAN)
					PIXEL(SCR_X_MIN+(Obj[i]->x>>6),198-(Obj[i]->y>>6))=1+rad;
	}
	rad=tic&31;
	if(rad<15 && ((!fzDead) || (fzDead && w_sp)))
		PIXEL(SCR_X_MIN+(x>>6),198-(y>>6))=129+rad;
}

void TMan::DrawMap(int i,int j){
	register char t=world[i][j];
	if(t>=128) t=175;
	else if(t>1 && t<=22) t=(t&3)+146;
	else if(t>22) t=51-(t>>1);
	register int yoff=198-j;
	PIXEL(SCR_X_MIN+i,yoff)=t;
}

void TMan::Resurrect(){
	pain=strcnt=fzWeap=speed=w_sp=ang=fzStep=0;
	health=MAX_HEALTH; SCR_Y_MID=59; RndTelep();
}

void TMan::StateInf(){
	int x_inf=SCR_X_MIN+65;
	String(STR_LEV_PASS_LEFT,x_inf,150);
	String(STR_GOLD_LEFT,x_inf,160);
	String(STR_TIME_LEFT,x_inf,170);
	if(NUM_MONS)
		String(STR_MONS_LEFT,x_inf,183);
	if(strcnt>1){
		strcnt--; String(INFO[nInfStr],SCR_X_MIN+5,6);
		if(nInfStr==23){
			if(nEnemy<=NUM_OPPONENT)
				String(NAME[nEnemy],SCR_X_MIN+5,12);
			else String(NAME_MONS[nEnemy-12],SCR_X_MIN+5,12);
			String(strcnt>>2,SCR_X_MIN+25,20);
		}
	}
	int az=angle+(ANGLE_45>>1); if(az>=ANGLE_360) az-=ANGLE_360; az/=75;
	ClrCharAt(7+az,SCR_X_MIN+93,132);
	ClrCharAt(3,SCR_X_MIN,126); String(nKill,SCR_X_MIN+6,126);
	String("$",SCR_X_MIN+64,126); String(cash,SCR_X_MIN+70,126);
	if(fzDead){ pain=0; return; }
	String(health>>5,SCR_X_MIN+35,126); String("%",SCR_X_MIN+53,126);
	if(nWeap>0){
		ClrCharAt(1,SCR_X_MIN+2,132); String(ammo[nWeap],SCR_X_MIN+8,132);
	}if(slip>0){
		ClrCharAt(4,SCR_X_MIN+30,132); String(slip>>4,SCR_X_MIN+35,132);
	}if(rent>0){
		ClrCharAt(5,SCR_X_MIN+50,132); String(rent>>6,SCR_X_MIN+55,132);
	}if(armor>0){
		ClrCharAt(6,SCR_X_MIN+70,132); String(armor>>7,SCR_X_MIN+76,132);
	}
	//////Расстояние до ближайшего соперника//////
	if(radar){
		int r,min_r=32000;
		for(int i=0; i<=NUM_OPPONENT; i++){
			r=Man[i]->r;
			if(i!=number && r<min_r) min_r=r;
		}
		i2a(min_r>>=6,_n_,2); _n_[2]='м'; _n_[3]=0; if(!min_r) _n_[1]='0';
		if((min_r<=3) && ((tic&7)<4))
			ClrCharAt(7,SCR_X_MIN+70,190);
		String(_n_,SCR_X_MIN+70,190);
	}
	///////////////////////////////////////////////
	if(invis){
		String(invis>>8,SCR_X_MIN+88,118);
		if(nWeap==0) invis--;  // если в руках кирка, невидимость
		else{                  // расходуется в 5 раз медленнее
			if(invis>=5) invis-=5;
			else invis=0;
		}
	}
	tex_x=SCR_X_MIN-1;
	if(pain){ pain--; tex_n=128; POSITIVE(); return; }
	if(rent && nWeap==3){
		tex_n=160; rent--; POSITIVE();
		if(slip && world[x>>6][y>>6]) slip--;
	}else if(slip && world[x>>6][y>>6]){
		--slip; tex_n=143; NEGATIVE();
	}else{
		if(invis){ tex_n=0; POSITIVE(); }
	}
	if(slip<48 && !world[x>>6][y>>6]) slip=0;
}

void TOMArEngine::Sell_Weapons(){     // Чужое/запрещенное оружие
	for(int i=0; i<=NUM_OPPONENT; i++){// автоматически продается за 2/3 цены
		for(int j=1; j<NUM_WEAP; j++){
			int am=Man[i]->ammo[j];
			if(am && LIC_WEA[j]!=3 && LIC_WEA[j]!=Man[i]->number){
				Man[i]->ammo[j]=0;
				float money=.6667*WEAP_PRICE[j]*am/AMMO_PACK[j]+.5;
				if((Man[i]->cash+=money)>30000) Man[i]->cash=30000;
			}
		}
	}
}
