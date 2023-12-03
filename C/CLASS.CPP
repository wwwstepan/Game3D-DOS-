#include "c\obj.h"
#include "c\wea.h"
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <mem.h>

// TBullet::TBullet():TVect(){
// int TObj::GetAngle(TVect &O){
// int TObj::Direction(TObj &O){
// void TObj::InSand(){
// void T1aid::Sprite(int ang){
// void TWeap::Sprite(int ang){
// void TItem::Sprite(int ang){
// void TGold::Sprite(int ang){
// void TBarrel::Sprite(int ang){
// void TCorpse::Sprite(int ang){
// void TMan::Sprite(int ang){
// void THol::Sprite(int ang){
// void TArm::Sprite(int ang){
// void TBarrelObl::Sprite(int){
// void TPul::Sprite(int ang){
// void TMis::Sprite(int ang){
// void TTaz::Sprite(int){
// void TBit::Sprite(int ang){
// void TMine::Sprite(int ang){
// void TMav::Sprite(int ang){
// void TShock::Sprite(int ang){
// void TExpl::Sprite(int){
// void TNap::Sprite(int){
// inline void sw(TObj **a, TObj **b){
// int sort(){
// int AddObj(TObj *New){
// void DelObj(int i){
// void TMan::Save(int hf){
// void T1aid::Save(int hf){
// void TWeap::Save(int hf){
// void TItem::Save(int hf){
// void TGold::Save(int hf){
// void TBarrel::Save(int hf){
// void TCorpse::Save(int hf){
// void TBarrelObl::Save(int hf){
// void TPul::Save(int hf){
// void TMis::Save(int hf){
// void TNap::Save(int hf){
// void TTaz::Save(int hf){
// void TBit::Save(int hf){
// void TMine::Save(int hf){
// void TMav::Save(int hf){
// void TShock::Save(int hf){
// void TExpl::Save(int hf){
// void TNExpl::Save(int hf){
// void TLiqExpl::Save(int hf){
// void TMov::Telep(int X,int Y){
// void TBeing::Telep(int X,int Y){
// void TMov::RndTelep(){
// int TMov::WallClip(){
// void TMov::Step(){
// void TBeing::SlipStep(){
// void TBeing::Step(){
// int TMan::Die(int power){
// void TSaveHeader::Get(TGameOption &O){
// void TSaveHeader::Load(TGameOption &O){
// void TBeing::Destroy(){
// void TBeing::Bits(char n){
// void TBeing::Blood(int speed,int time){

TBullet::TBullet():TVect(){
   nOwner=own_obj;
   if(world[x>>6][y>>6]) x-=COS(angle)>>5,y-=SIN(angle)>>5;
}

int TObj::GetAngle(TVect &O){
   int a=Angle(x-O.x,y-O.y); a-=O.angle;
   if(a<=-ANGLE_180) a+=ANGLE_360;
   return a;
}

int TObj::Direction(TObj &O){
   int a=Angle(O.x-x,O.y-y);
   if(a<0) a+=ANGLE_360;
   return a;
}

void TObj::InSand(){
   char &w=world[x>>6][y>>6];
   if(w>=23 && w<=28){ if(--w<=22) w=0; }
}

void T1aid::Sprite(int ang){
   mod_lower=0; tex_show=1; mod_sc=r; tex_index=75;
   mod_turn=items_turn_ang-ang;
}
void TWeap::Sprite(int ang){
   mod_lower=h; tex_show=1; mod_sc=r; tex_index=30+type;
   mod_turn=ang-items_turn_ang;
}
void TItem::Sprite(int ang){
   mod_lower=0; tex_show=1; mod_sc=r; tex_index=69+type;
   mod_turn=items_turn_ang-ang;
}
void TGold::Sprite(int ang){
   int t=price>>6; if(t>3) t=3; mod_lower=1280; tex_show=1;
   mod_sc=r; tex_index=26+t; mod_turn=items_turn_ang-ang;
}
void TBarrel::Sprite(int ang){
   mod_lower=0; tex_show=1; mod_sc=r; mod_turn=-ang;
   tex_index=(type<3)?68:78;
}
void TCorpse::Sprite(int ang){
   mod_lower=0; tex_show=1; mod_sc=r;
   tex_index=(n<<3)+6;
   if(cnt<1000) cnt++; else tex_index++;
   mod_turn=angle-ang;
}
void TMan::Sprite(int ang){
   if(invis) tex_show=99;
   else{
      mod_lower=0; tex_show=1; mod_turn=angle-ang;
      mod_sc=r; tex_index=(number<<3);
      if(fzDead){ tex_index+=(2+(fzDead>>2)); if(_life==0) tex_show=99; }
      else if(pain) tex_index+=2; else tex_index+=fzStep>>2;
   }
}
void THol::Sprite(int ang){
   if(Man[num]->fzDead || !vis){ vis=0; tex_show=99; }
   else{
      mod_lower=0; tex_show=1; mod_turn=angle-ang;
      mod_sc=r; tex_index=(num<<3);
      tex_index+=Man[num]->fzStep>>2;
   }
}
void TArm::Sprite(int ang){
   if((Man[num]->invis && N_MAN_DRAW!=num && n_hol) || Man[num]->fzDead || !vis)
      tex_show=99;
   else{
      mod_lower=-2500; tex_show=1; mod_sc=r; int nw=Man[num]->nWeap;
      if(nw<0) tex_show=99; else tex_index=30,tex_index+=nw;
      if(Man[num]->fzWeap>1) tex_index+=15; mod_turn=angle-ang;
   }
}
void TBarrelObl::Sprite(int){
   mod_lower=0; tex_show=1; mod_sc=r; tex_index=60+(tic&1);
   mod_turn=random(ANGLE_360);
}
void TPul::Sprite(int ang){
   mod_lower=0; tex_show=1; mod_sc=r; tex_index=95; mod_turn=angle-ang;
}
void TMis::Sprite(int ang){
   mod_lower=0; tex_show=1; mod_sc=r; tex_index=94; mod_turn=angle-ang;
}
void TTaz::Sprite(int){
   mod_lower=0; tex_show=1; mod_sc=r; tex_index=77; mod_turn=random(ANGLE_360);
}
void TBit::Sprite(int ang){
   mod_lower=h; tex_show=1; mod_sc=r; tex_index=84+type; mod_turn=angle-ang;
}
void TMine::Sprite(int ang){
   mod_lower=h; tex_show=1; mod_sc=r; tex_index=37; mod_turn=angle-ang;
}
void TMav::Sprite(int ang){
   mod_lower=0; tex_show=1; mod_sc=r; tex_index=93; mod_turn=angle-ang;
}
void TShock::Sprite(int ang){
   mod_lower=0; tex_show=1; mod_sc=r; tex_index=80+(rand()&3);
   mod_turn=angle-ang;
}
void TExpl::Sprite(int){
   if(type&EXPL_INV) tex_show=99;
   else if(fz<9){
      tex_scale=size/r; tex_top=-(tex_scale>>1);
      tex_show=0; tex_ptr=pBurst+(fz<<10);
   }
}
void TNap::Sprite(int){
   tex_scale=11000/r; tex_top=-(tex_scale>>1);
   tex_n=0; tex_show=0; tex_ptr=pBurst+7168+(((tic&15)>>3)<<10);
}

inline void sw(TObj **a, TObj **b){
   register TObj *TempP=*a; *a=*b; *b=TempP;
}

int sort(){
   register int i,n; int j;
   for(j=0; j<NUM_OBJ; j++){
      for(n=j,i=j+1; i<=NUM_OBJ; i++)
         if(Obj[i]->r<Obj[n]->r) n=i;
      if(Obj[n]->r>1500) break;
      sw(&Obj[j],&Obj[n]);
   }return j;
}

int AddObj(TObj *New){
   if(New==NULL) return 0;
   if(NUM_OBJ<MAX_NUM_OBJ){ Obj[++NUM_OBJ]=New; return 1; }
   delete New; New=NULL; return 0;
}

void DelObj(int i){
   if(Obj[i]!=NULL) delete Obj[i];
   Obj[i]=Obj[NUM_OBJ];
   Obj[NUM_OBJ--]=NULL;
}

void TMan::Save(int hf){
   char c=T_MAN; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void T1aid::Save(int hf){
   char c=T_1AID; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void TWeap::Save(int hf){
   char c=T_WEAP; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void TItem::Save(int hf){
   char c=T_ITEM; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void TGold::Save(int hf){
   char c=T_GOLD; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void TBarrel::Save(int hf){
   char c=T_BARREL; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void TCorpse::Save(int hf){
   char c=T_CORPSE; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void TBarrelObl::Save(int hf){
   char c=T_BARRELOBL; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void TPul::Save(int hf){
   char c=T_PUL; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void TMis::Save(int hf){
   char c=T_MIS; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void TNap::Save(int hf){
   char c=T_NAP; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void TTaz::Save(int hf){
   char c=T_TAZ; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void TBit::Save(int hf){
   char c=T_BIT; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void TMine::Save(int hf){
   char c=T_MINE; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void TMav::Save(int hf){
   char c=T_MAV; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void TShock::Save(int hf){
   char c=T_SHOCK; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void TExpl::Save(int hf){
   char c=T_EXPL; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void TNExpl::Save(int hf){
   char c=T_NEXPL; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}
void TLiqExpl::Save(int hf){
   char c=T_LIQEXPL; _write(hf,&c,1); char *p=(char*)this; _write(hf,p+2,sizeof(*this)-2);
}

void TMov::Telep(int X,int Y){
   if(X>64 && Y>64 && X<(WORLD_X_SIZE-64) && Y<(WORLD_Y_SIZE-64))
      world[X>>6][Y>>6]=0;
   x=X; y=Y; mv_angle=mv_speed=0;
}

void TBeing::Telep(int X,int Y){
   if(X>64 && Y>64 && X<(WORLD_X_SIZE-64) && Y<(WORLD_Y_SIZE-64))
      world[X>>6][Y>>6]=0;
   x=X; y=Y; _life=105; mv_angle=mv_speed=speed=0;
   x_obj=x; y_obj=y; r_obj=60; pow_obj=255; t_obj=EXPL_TEL;
   sz_obj=10000; own_obj=number; fz_obj=3; AddObj(new TExpl);
}

void TMov::RndTelep(){
   register int i; int j=3000,X,Y,Obj_in_way;
   do{
      X=random(WORLD_COL-2); Y=random(WORLD_ROW-2); X++; Y++;
      for(i=0, Obj_in_way=0; i<=NUM_OBJ && !Obj_in_way; i++)
         if((((Obj[i]->x)>>6)==X) && (((Obj[i]->y)>>6)==Y))
            Obj_in_way=1;
   }while((world[X][Y] || Obj_in_way) && (--j)>0);
   if(j<=0){
      for(X=1; X<WORLD_COL-1; X++)
         for(Y=1; Y<WORLD_ROW-1; Y++)
            if(!world[X][Y]){
               X<<=6; X+=32; Y<<=6; Y+=32; Telep(X,Y); return;
            }
   }
   X<<=6; X+=32; Y<<=6; Y+=32; Telep(X,Y);
}

int TMov::WallClip(){
   int X,Y,_X,_Y,x_s,y_s;
   if(world[x>>6][y>>6]) return 1;
   else{
      X=x>>6; x_s=x&63; _X=x&0xFFC0; Y=y>>6; y_s=y&63; _Y=y&0xFFC0;
      if(world[X][Y+1]>0 && y_s>_OVERBOARD) y=_Y+_OVERBOARD;
      if(world[X][Y-1]>0 && y_s<OVERBOARD)  y=_Y+OVERBOARD;
      if(world[X+1][Y]>0 && x_s>_OVERBOARD) x=_X+_OVERBOARD;
      if(world[X-1][Y]>0 && x_s<OVERBOARD)  x=_X+OVERBOARD;
      if(x_s<OVERBOARD && y_s<OVERBOARD)
         if(world[X-1][Y-1]>0 && world[X][Y-1]<1 && world[X-1][Y]<1)
            x=_X+OVERBOARD,y=_Y+OVERBOARD;
      if(x_s<OVERBOARD && y_s>_OVERBOARD)
         if(world[X-1][Y+1]>0 && world[X][Y+1]<1 && world[X-1][Y]<1)
            x=_X+OVERBOARD,y=_Y+_OVERBOARD;
      if(x_s>_OVERBOARD && y_s>_OVERBOARD)
         if(world[X+1][Y+1]>0 && world[X][Y+1]<1 && world[X+1][Y]<1)
            x=_X+_OVERBOARD,y=_Y+_OVERBOARD;
      if(x_s>_OVERBOARD && y_s<OVERBOARD)
         if(world[X+1][Y-1]>0 && world[X][Y-1]<1 && world[X+1][Y]<1)
            x=_X+_OVERBOARD,y=_Y+OVERBOARD;
   }return 0;
}

void TMov::Step(){
   int i,old_x,old_y;
   if(mv_speed>25) mv_speed=25; else if(mv_speed<-25) mv_speed=-25;
   old_x=x; old_y=y;
   x+=(COS(mv_angle)*mv_speed)>>9; y+=(SIN(mv_angle)*mv_speed)>>9;
   if(mv_speed>0) mv_speed-=mv_speed<5?mv_speed:5;
   else if(mv_speed<0) mv_speed-=mv_speed>-5?mv_speed:-5;
   if(WallClip()) x=old_x,y=old_y;
   int x_index=(x-(COS(mv_angle)>>4))>>6,y_index=(y-(SIN(mv_angle)>>4))>>6;
   if(world[x_index][y_index]>=128) RndTelep();
   for(i=0; i<=NUM_OBJ; i++)
      if(Obj[i]->Its()>31 && Obj[i]!=this && R(*Obj[i])<50)
         { x=old_x,y=old_y; Obj[i]->Push(mv_angle,mv_speed<<1); return; }
}

void TBeing::SlipStep(){
   int i,old_x,old_y,dx,dy;
   if(mv_speed>9) mv_speed=9; else if(mv_speed<-9) mv_speed=-9;
   old_x=x; old_y=y;
   x+=(COS(mv_angle)*mv_speed)>>9; y+=(SIN(mv_angle)*mv_speed)>>9;
   mv_speed=0;
   if(x<89) x=89; else if(x>WORLD_X_SIZE-89) x=WORLD_X_SIZE-89;
   if(y<89) y=89; else if(y>WORLD_Y_SIZE-89) y=WORLD_Y_SIZE-89;
   for(i=0; i<=NUM_OBJ; i++)
      if(Obj[i]->Its()>31 && Obj[i]!=this && R(*Obj[i])<50)
         { x=old_x; y=old_y; Obj[i]->Push(angle,speed<<1); return; }
   dx=(COS(angle)*speed)>>9; dy=(SIN(angle)*speed)>>9;
   if(world[x>>6][y>>6] && abs(dx)>1 && abs(dy)>1){ dx>>=1; dy>>=1; }
   old_x=x; old_y=y; x+=dx; y+=dy;
   if(x<89) x=89; else if(x>WORLD_X_SIZE-89) x=WORLD_X_SIZE-89;
   if(y<89) y=89; else if(y>WORLD_Y_SIZE-89) y=WORLD_Y_SIZE-89;
   for(i=0; i<=NUM_OBJ; i++)
      if(Obj[i]->Its()>31 && Obj[i]!=this && R(*Obj[i])<50)
         { x=old_x; y=old_y; Obj[i]->Push(angle,speed<<1); return; }
}

void TBeing::Step(){
   int i,old_x,old_y,dx,dy;
   if(mv_speed>30) mv_speed=30; else if(mv_speed<-30) mv_speed=-30;
   old_x=x; old_y=y;
   x+=(COS(mv_angle)*mv_speed)>>9; y+=(SIN(mv_angle)*mv_speed)>>9;
   if(mv_speed>0) mv_speed-=mv_speed<4?mv_speed:4;
   else if(mv_speed<0) mv_speed-=mv_speed>-4?mv_speed:-4;
   if(WallClip()) x=old_x,y=old_y;
   for(i=0; i<=NUM_OBJ; i++)
      if(Obj[i]->Its()>31 && Obj[i]!=this && R(*Obj[i])<50)
         { x=old_x,y=old_y; Obj[i]->Push(angle,speed<<1); return; }
   dx=(COS(angle)*speed)>>9; dy=(SIN(angle)*speed)>>9;
   if(fzDead && abs(dx)>1 && abs(dy)>1){ dx>>=1; dy>>=1; }
   old_x=x; old_y=y; x+=dx; y+=dy;
   if(WallClip()) x=old_x,y=old_y;
   for(i=0; i<=NUM_OBJ; i++)
      if(Obj[i]->Its()>31 && Obj[i]!=this && R(*Obj[i])<50)
         { x=old_x,y=old_y; Obj[i]->Push(angle,speed<<1); return; }
}

int TMan::Die(int power){
   int tele_expl=(power>250);
   power<<=7;
   power/=MAN_HLTH[number];
   if(armor && health<1600){
      if((armor-=power)<0) armor=0;
   }else if(health>0){
      if((health-=power)<=0){
         if(tele_expl) health=1;
         else{
            Dead();
            return (GAME_TYPE!=COMMAND_GAME)?MAN_PRICE[number]:1;
         }
      }
   }
   if(tele_expl){ RndTelep(); pain=100; }
   else{
      if(power>100) pain=17; // попали из ружья
      else if(power>40) pain=11;
      else pain=7;
   }
   return 0;
}

void TSaveHeader::Get(TGameOption &O){
   setmem(this,sizeof(*this),0);
   OMAr=OMAr_CODE; new_str=0x0A0D;
   num_ver=NUM_VERSION;
   gold_left=GOLD_LEFT;
   levels_pass=LEVELS_PASS;
   time_left=TIME_LEFT;
   game_type=GAME_TYPE;
   is_monsters=IS_MONSTERS;
   levels_to_go=O.levels_to_go;
   level_type=LEVEL_TYPE;
   time_limit=O.time_limit;
   for(int i=0; i<NUM_WEAP; i++)
      lic_wea[i]=LIC_WEA[i];
   for(i=0; i<3; i++){
      ix_scr[i]=IX_SCR[i];
      control[i]=CONTROL[i];
      man_price[i]=MAN_PRICE[i];
      man_hlth[i]=MAN_HLTH[i];
      killed[i]=KILLED[i];
   }
   for(i=0; i<=NUM_OPPONENT; i++){
      hologr[i]=Hol[i]->vis; hx[i]=Hol[i]->x; hy[i]=Hol[i]->y;
   }
   cur_lev=CUR_LEV;
   prev_lev=PREV_LEV; prev2_lev=PREV2_LEV; prev3_lev=PREV3_LEV;
   next_lev=NEXT_LEV;
}

void TSaveHeader::Load(TGameOption &O){
   NUM_OBJ=-1; NUM_OPPONENT=-1; ITEMS_LEFT=0;
   GOLD_LEFT=gold_left;
   LEVELS_PASS=levels_pass;
   LEVEL_TYPE=level_type;
   TIME_LEFT=time_left;
   GAME_TYPE=game_type;
   IS_MONSTERS=is_monsters;
   O.levels_to_go=levels_to_go;
   O.time_limit=time_limit;
   for(int i=0; i<NUM_WEAP; i++)
      LIC_WEA[i]=lic_wea[i];
   for(i=0; i<3; i++){
      IX_SCR[i]=ix_scr[i];
      CONTROL[i]=control[i];
      MAN_PRICE[i]=man_price[i];
      MAN_HLTH[i]=man_hlth[i];
      KILLED[i]=killed[i];
   }
   CUR_LEV=cur_lev;
   PREV_LEV=prev_lev; PREV2_LEV=prev2_lev; PREV3_LEV=prev3_lev;
   NEXT_LEV=next_lev; if(NEXT_LEV>=NUM_LEVELS) NEXT_LEV=0;
}

void TBeing::Destroy(){
   x_obj=x; y_obj=y;
   for(int i=0; i<5; i++){
      pow_obj=(rand()&15)+11; t_obj=(rand()&255)+90;
      n_obj=4+(rand()&3); ang_obj=random(ANGLE_360);
      AddObj(new TBit);
   }
}

void TBeing::Bits(char n){
   x_obj=x; y_obj=y;
   while(n--){
      pow_obj=(rand()&15)+10; t_obj=(rand()&511)+99;
      n_obj=rand()&3; ang_obj=random(ANGLE_360);
      AddObj(new TBit);
   }
}

void TBeing::Blood(int speed,int time){
   x_obj=x; y_obj=y; n_obj=0; pow_obj=speed; t_obj=time;
   ang_obj=random(ANGLE_360); AddObj(new TBit);
}
