#include "c\monster.h"
#include "c\engine.h"
#include "c\wea.h"
#include <dos.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>

// void TOMArEngine::SetArmPosition(){
// void TOMArEngine::LoadWorld(int n_wich){
// int TOMArEngine::GetNumber(char *s){
// void TOMArEngine::SaveGame(char far *file_name){
// void TOMArEngine::LoadGame(char *fname){
// void TOMArEngine::NewItem(){
// void TOMArEngine::InformStr(){
// void TOMArEngine::ClearStrings(){
// void TOMArEngine::LoadScreen(unsigned long nScr){
// void TOMArEngine::SaveLoadName(int sav){
// 
void TOMArEngine::SetArmPosition(){
   for(int i=0; i<=NUM_OPPONENT; i++){
      int a=Man[i]->angle;
      Arm[i]->x=Man[i]->x+(COS(a)*5>>7); Arm[i]->y=Man[i]->y+(SIN(a)*5>>7);
      Arm[i]->angle=hArm[i]->angle=a;
      if(Hol[i]->vis){
         hArm[i]->x=Hol[i]->x+(COS(a)*5>>7); hArm[i]->y=Hol[i]->y+(SIN(a)*5>>7);
         if(!Man[i]->ammo[HOL_N] || Man[i]->fzDead)
            Hol[i]->vis=hArm[i]->vis=0;
         else{
            Hol[i]->angle=a;
            if(!(CADR_NUMBER&15))
               Man[i]->ammo[HOL_N]--;
         }
      }
   }
}

void TOMArEngine::LoadWorld(int n_wich){
   int i,j,x,y,n;
   if(n_wich>=0 && n_wich<NUM_LEVELS)
      CUR_LEV=n_wich;
   else{
      i=0;
      do{
         n=random(NUM_LEVELS);
         if(++i>5000) break;
      }while(n==CUR_LEV || n==PREV_LEV || n==PREV2_LEV || n==PREV3_LEV);
      PREV3_LEV=PREV2_LEV; PREV2_LEV=PREV_LEV; PREV_LEV=CUR_LEV;
      CUR_LEV=NEXT_LEV; NEXT_LEV=n;
   }

   for(i=0; i<=NUM_OBJ; i++){ delete Obj[i]; Obj[i]=0L; }
   for(i=0; i<=NUM_OPPONENT; i++)
      Man[i]=0L,Arm[i]=0L,Hol[i]=0L,hArm[i]=0L;
   NUM_OPPONENT=NUM_OBJ=-1;
   ////////////////////////
   int hf=_open(RES_FILE_NAME,O_RDONLY); if(hf==-1) Error(STR_DTA[5]);
   lseek(hf,FTR.ofs_levels+((long)CUR_LEV*LEVEL_DISK_SIZE),0);
   _read(hf,world,LEVEL_DISK_SIZE);
   _close(hf);
   ////////////////////////
   for(i=0; i<Options.n_man; i++){
      n_obj=++NUM_OPPONENT; ang_obj=random(ANGLE_360);
      Man[n_obj]=new TMan;
      t_obj=1; Arm[n_obj]=new TArm;
      Hol[n_obj]=new THol;
      t_obj=0; hArm[n_obj]=new TArm;
      if(!Man[n_obj] || !Arm[n_obj] || !Hol[n_obj] || !hArm[n_obj])
         Error(STR_DTA[6]);
      AddObj(Man[n_obj]); AddObj(Arm[n_obj]);
      AddObj(Hol[n_obj]); AddObj(hArm[n_obj]);
      ManInform[n_obj].MakeMan();
   }
   ITEMS_LEFT=0;
   LEVEL_TYPE=1;//rand()%3;// Тип уровня 0     -засыпается
   int pr,nm,nm_all;
   nm_all=100;
   for(i=0; i<=NUM_OPPONENT; i++)
      nm_all+=(rand()%101)+MAN_PRICE[i];
   for(nm=i=0; i<WORLD_COL; i++)
      for(j=0; j<WORLD_ROW; j++)
         if(world[i][j]<128){
            if(world[i][j]>22) nm_all++; // кол-во золота зависит
            if(world[i][j]>25) nm_all++; // от кол-ва песка на уровне
         }
   GOLD_LEFT=0; nm=nm_all;
   for(i=30000; nm>0 && i>0; i--){
      x=random(WORLD_COL-2); y=random(WORLD_ROW-2); x++; y++;
      if(world[x][y]>WALL_SAND && world[x][y]<128){
         x_obj=(x<<6)+16+(rand()&31); y_obj=(y<<6)+16+(rand()&31);
         int type_sand=world[x][y]-WALL_SAND;
         t_obj=random(101)+type_sand*25;
         //if((GOLD_LEFT+t_obj)>nm_all) t_obj=nm_all-GOLD_LEFT;
         if(AddObj(new TGold)) nm-=t_obj,GOLD_LEFT+=t_obj;
         if(GOLD_LEFT>=nm_all) break;
      }
   }
   if(nm>0) for(i=30000; nm>0 && i>0; i--){
      x=random(WORLD_COL-2); y=random(WORLD_ROW-2); x++; y++;
      if(!world[x][y]){
         x_obj=(x<<6)+16+(rand()&31); y_obj=(y<<6)+16+(rand()&31);
         t_obj=1+random(200);
         if((GOLD_LEFT+t_obj)>nm_all) t_obj=nm_all-GOLD_LEFT;
         if(AddObj(new TGold)) nm-=t_obj,GOLD_LEFT+=t_obj;
         if(GOLD_LEFT>=nm_all) break;
      }
   }
   /////////////////////////////////
   NUM_MONS=NUM_INCUB=0;
   IS_MONSTERS=0; // добавить монстров
   if(!NUM_OPPONENT) GAME_TYPE=COMMAND_GAME;
   if(!NUM_OPPONENT || GAME_TYPE==COMMAND_GAME || (GAME_TYPE!=NOMONSTERS_GAME && !(rand()%3))){
      IS_MONSTERS=1;
      nm=6+(rand()&7);
      for(i=0; i<nm; i++){
         x_obj=0; // если 0, то в случайное место
         n_obj=MIN_MONSTER_TYPE+(rand()%MONST_TYPES);
         AddObj(new TMonster);
      }
   }
   ///// добавить бочки ///////////
   int kol_barrel[8]={20,40,80,100,150,200,250,300};
   int nm_div=kol_barrel[rand()&7];
   for(nm=i=0; i<WORLD_COL; i++)
      for(j=0; j<WORLD_ROW; j++)
         if(!world[i][j]) nm++;
   nm/=nm_div; nm++;
   TBarrel *brl;
   while(nm--){
      t_obj=rand()&7;
      brl=new TBarrel; brl->RndTelep(); AddObj(brl);
   }
   for(i=0; i<3; i++){ KILLED[i]=0; KILL[i]=0; }
   CADR_NUMBER=0L;
   TIME_LEFT=Options.time_limit;
   if(NUM_OPPONENT<0 || NUM_OBJ<0)
      Error(STR_DTA[7]);
   for(i=0; i<=NUM_OPPONENT; i++)
      Man[i]->RndTelep();
   remove(PREV_FILE_NAME);
   rename(LAST_FILE_NAME,PREV_FILE_NAME);
   SaveGame(LAST_FILE_NAME);
}

int TOMArEngine::GetNumber(char *s){
   char s2[50];
   for(int i=2; i<127 && s[i]!='='; i++);
   while(!isfnum(s[++i]));
   for(int j=0; (isfnum(s[i]) && j<49); j++,i++)
      s2[j]=s[i];
   s2[j]=0;
   return atoi(s2);
}

void TOMArEngine::SaveGame(char far *file_name){
   int hf=_creat(file_name,0); if(hf<0) return;
   TSaveHeader Header;
   Header.Get(Options);
   _write(hf,&Header,sizeof(TSaveHeader));
   _write(hf,KILL,3);
   _write(hf,NAME,sizeof(NAME));
   _write(hf,world,LEVEL_DISK_SIZE);
   for(int i=0; i<=NUM_OBJ; i++)
      Obj[i]->Save(hf);
   unsigned long end_of_file=0x7F7F7F7F;
   _write(hf,&end_of_file,4);
   _close(hf);
   for(i=0; i<=NUM_OPPONENT; i++) Man[i]->Info(27);
}

void TOMArEngine::LoadGame(char *fname){
   int i,j,hf=_open(fname,O_RDONLY); if(hf==-1) return;
   TSaveHeader Header;
   _read(hf,&Header,sizeof(TSaveHeader));
   _read(hf,KILL,3);
   if(Header.Check()) return;
   for(i=0; i<=NUM_OBJ; i++){ delete Obj[i]; Obj[i]=0L; }
   for(i=0; i<=NUM_OPPONENT; i++)
      Man[i]=0L,Arm[i]=0L,Hol[i]=0L,hArm[i]=0L;
   Header.Load(Options);
   _read(hf,NAME,sizeof(NAME));
   _read(hf,world,LEVEL_DISK_SIZE);
   char far *p=0L;
   NUM_MONS=NUM_INCUB=0; char c=0;
   while(c!=T_EOF && NUM_OBJ<MAX_NUM_OBJ){
      _read(hf,&c,1);
      switch(c){
      case T_1AID:
         AddObj(new T1aid); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(T1aid)-2); ITEMS_LEFT++; break;
      case T_WEAP:
         AddObj(new TWeap); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TWeap)-2); ITEMS_LEFT++; break;
      case T_ITEM:
         AddObj(new TItem); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TItem)-2); ITEMS_LEFT++; break;
      case T_BARREL:
         AddObj(new TBarrel); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TBarrel)-2); break;
      case T_CORPSE:
         AddObj(new TCorpse); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TCorpse)-2); break;
      case T_MAN:
         AddObj(new TMan); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TMan)-2);
         n_obj=((TMan*)Obj[NUM_OBJ])->number;
         if(++NUM_OPPONENT>2 || n_obj>2)
            Error(STR_DTA[8]);
         Man[n_obj]=(TMan*)Obj[NUM_OBJ];
         t_obj=1; Arm[n_obj]=new TArm(); AddObj(Arm[n_obj]);
         Hol[n_obj]=new THol(); AddObj(Hol[n_obj]);
         t_obj=0; hArm[n_obj]=new TArm(); AddObj(hArm[n_obj]);
         break;
      case T_MONSTER:
         AddObj(new TMonster); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TMonster)-2); break;
      case T_INCUBATOR:
         AddObj(new TIncubator); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TIncubator)-2); break;
      case T_BARRELOBL:
         AddObj(new TBarrelObl); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TBarrelObl)-2); break;
      case T_PUL:
         AddObj(new TPul); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TPul)-2); break;
      case T_MIS:
         AddObj(new TMis); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TMis)-2); break;
      case T_MAV:
         AddObj(new TMav); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TMav)-2); break;
      case T_TAZ:
         AddObj(new TTaz); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TTaz)-2); break;
      case T_NAP:
         AddObj(new TNap); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TNap)-2); break;
      case T_SHOCK:
         AddObj(new TShock); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TShock)-2); break;
      case T_BIT:
         AddObj(new TBit); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TBit)-2); break;
      case T_EXPL:
         AddObj(new TExpl); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TExpl)-2); break;
      case T_NEXPL:
         AddObj(new TNExpl); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TNExpl)-2); break;
      case T_MINE:
         AddObj(new TMine); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TMine)-2); break;
      case T_GOLD:
         AddObj(new TGold); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TGold)-2); break;
      case T_LIQEXPL:
         AddObj(new TLiqExpl); p=(char*)Obj[NUM_OBJ];
         _read(hf,p+2,sizeof(TLiqExpl)-2); break;
      }
   }
   _close(hf);
   Options.n_man=NUM_OPPONENT+1;
   for(i=0; i<=NUM_OPPONENT; i++){
      hArm[i]->vis=Hol[i]->vis=Header.hologr[i];
      Hol[i]->x=Header.hx[i]; Hol[i]->y=Header.hy[i];
      Man[i]->Info(28);
   }
   CADR_NUMBER=0L;
}

void TOMArEngine::NewItem(){
   int i=10000;
   do{
      x_obj=1+random(WORLD_COL-2);
      y_obj=1+random(WORLD_ROW-2);
   }while(--i>0 && world[x_obj][y_obj]);
   if(i<=0) return;
   x_obj=(x_obj<<6)+(rand()&31)+16; y_obj=(y_obj<<6)+(rand()&31)+16;
   switch(rand()&3){
      case 0: if(AddObj(new T1aid)) ITEMS_LEFT++; break;
      case 1: t_obj=(random(5))+1; if(AddObj(new TItem)) ITEMS_LEFT++; break;
      default:
         for(i=256; i>0; i--){
            t_obj=(random(NUM_WEAP));
            if(LIC_WEA[t_obj]<4){
               n_obj=0; h_obj=-13000;
               r_obj=NUM_AMMO[t_obj];
               r_obj+=random(PLUS_AMMO[t_obj]);
               if(AddObj(new TWeap)) ITEMS_LEFT++;
               i=0;
            }
         }
   }
}

void TOMArEngine::InformStr(){
   i2a(LEVELS_PASS,STR_LEV_PASS_LEFT,2);
   STR_LEV_PASS_LEFT[2]='/';
   i2a(Options.levels_to_go,STR_LEV_PASS_LEFT+3,2);
   i2a(GOLD_LEFT,STR_GOLD_LEFT+1,4);
   i2a(NUM_MONS,STR_MONS_LEFT,2);
   STR_MONS_LEFT[2]=' ';
   UNS m,s; s=TIME_LEFT/18.2; m=s/60; s=_DX;
   *((UNS*)(STR_TIME_LEFT+3))=*((UNS*)STR_TIME_LEFT)=0x3030;
   m/=10; STR_TIME_LEFT[1]+=_DL; STR_TIME_LEFT[0]+=m;
   s/=10; STR_TIME_LEFT[4]+=_DL; STR_TIME_LEFT[3]+=s;
}

void TOMArEngine::ClearStrings(){
   tex_ptr=pFacet+9984;
   DRAW_BACK();
}

void TOMArEngine::LoadScreen(unsigned long nScr){
   Clear64K(GrArea);
   int hf=_open(RES_FILE_NAME,O_RDONLY); if(hf==-1) Error(STR_DTA[5]);
   nScr<<=16; nScr+=FTR.ofs_screens; lseek(hf,nScr,0);
   char pal[768];
   float r,g,b; float gam=Options.gamma/100.0;
   _read(hf,pal,768);
   for(int i=0; i<256; i++){
      _read(hf,pal,3); r=pal[0]*gam; g=pal[1]*gam; b=pal[2]*gam;
      if(r>63.0) r=63; if(g>63.0) g=63; if(b>63.0) b=63;
      RGB_COLOR[i].Put(r,g,b); SetPal(i);
   }
   _read(hf,GrBuff,64000U);
   _close(hf);
}

void TOMArEngine::SaveLoadName(int sav){
   char s[15]; s[0]=0;
   if(InputStr(sav?"Сохранить под именем:":"Загрузить игру:",s,8)){
      AddSavExt(s);
      if(sav) SaveGame(s);
      else LoadGame(s);
   }
}
