#include <io.h>
#include <fcntl.h>
#include <dos.h>
#include <dir.h>
#include <math.h>
#include <stdlib.h>
#include "c\3d.h"
#include "c\obj.h"
#include "c\engine.h"

// void TOMArEngine::Logo(){
// void TOMArEngine::ChangeHealth(){
// char Change_level=0;
// void TOMArEngine::Shop(){
// void TOMArEngine::GameOver(){
// void TOMArEngine::Run(){

void TOMArEngine::Logo(){
	AllocMem(); MakeTables(); LoadRes();
#if !IS_DEBUG
	CheckEXE();
#endif
	mode(19); LoadScreen(SCREEN_LOGO);
	SWBUFF(); LoadOptions(); shape_view.GetMaxMem();
	for(int n=raw_key=0; n<30 && !raw_key; n++) delay(100);
	GrowDark(1,1);
}
void TOMArEngine::ChangeHealth(){
#if 0
	int i; double new_h[3],av=0.;
	for(i=0; i<=NUM_OPPONENT; i++)
		av+=(KILLED[i]-KILL[i]);
	av/=(NUM_OPPONENT+1);
	for(i=0; i<=NUM_OPPONENT; i++)
		new_h[i]=MAN_HLTH[i]+((KILLED[i]-KILL[i]-av)*4.567);
	for(i=0; i<=NUM_OPPONENT; i++){
		MAN_HLTH[i]=new_h[i];
		if(MAN_HLTH[i]<7) MAN_HLTH[i]=7;
		else if(MAN_HLTH[i]>128) MAN_HLTH[i]=128;
	}
#endif
}//TOMArEngine::ChangeHealth()

char Change_level=0;

void TOMArEngine::Shop(){
	int n,i,j,iii; mod_turn=0;
	ShowWhyIsEndLevel();
	Sell_Weapons();
	GAME_IS_GO=0;
	for(i=0; i<SHOP_LEVEL; i++){ // колебания цен
		WEAP_PRICE[i]=((double)WEAP_PRICE_BASE[i])*(((double)(rand()&255))/255.+.5);
		if(WEAP_PRICE[i]<1) WEAP_PRICE[i]=1;
	}
	if(NUM_OPPONENT>0)
		ChangeHealth(); // изменить выносливость
	for(i=0; i<=NUM_OPPONENT; i++) Man[i]->GetInfo(ManInform[i]);
	LoadScreen(SCREEN_SHOP);
	SWBUFF();
	int NumOk;
	for(iii=0; iii<=NUM_OPPONENT; iii++){
		for(j=0; j<NUM_WEAP && LIC_WEA[j]!=ManInform[iii].number && LIC_WEA[j]!=3; j++);
		ManInform[iii].nBuyWea=j;
		if(ManInform[iii].cash<1){  //если нет денег, показать
			for(j=1; j<NUM_WEAP; j++)//имеющееся оружие, если нет оружия-на выход
				if(ManInform[iii].ammo[j]){ ManInform[iii].nBuyWea=j; break; }
			if(j==NUM_WEAP) ManInform[iii].nBuyWea=SHOP_EXIT;
		}
	}
	Change_level=15;
	do{
		if(Change_level<15){
			if(Change_level)
				Change_level--;
		}else{
			Change_level--;
			////////////////////////
			i=0;
			do{
				n=random(NUM_LEVELS);
				if(++i>5000) break;
			}while(n==CUR_LEV || n==PREV_LEV || n==PREV2_LEV || n==PREV3_LEV);
			PREV3_LEV=PREV2_LEV; PREV2_LEV=PREV_LEV; PREV_LEV=CUR_LEV;
			CUR_LEV=NEXT_LEV; NEXT_LEV=n;
			int hf=_open(RES_FILE_NAME,O_RDONLY); if(hf==-1) Error(STR_DTA[5]);
			lseek(hf,FTR.ofs_levels+((long)NEXT_LEV*LEVEL_DISK_SIZE),0);
			_read(hf,world,LEVEL_DISK_SIZE);
			_close(hf);
			////////////////////////
			for(iii=0; iii<=NUM_OPPONENT; iii++){
				int x=MAN_SCR_X[IX_SCR[iii]];
				bar(x,169,x+100,198,0);
				ClrCharAt(3,x+2,170); String(ManInform[iii].nKill,x+10,170);
				String("Доход",x+2,177); String(ManInform[iii].doh,x+40,177);
				String("Выносливость",x+2,184); String(MAN_HLTH[iii]/1.2795,x+78,184);
				int lev_to_go=Options.levels_to_go-LEVELS_PASS+1;
				String("Еще",x+2,191); String(lev_to_go,x+33,191);
				int o10=lev_to_go%10;
				String("уров",x+58,191);
				String(o10==1?"ень":(o10>0 && o10<5)?"ня":"ней",x+82,191);
				// карта
				bar(x,93,x+101,101,5);
				String("Следующий уровень",x,95);
				for(i=0; i<WORLD_COL; i++)
					for(j=0; j<WORLD_ROW; j++){
						register char t=world[i][j];
						if(t>=128) t=175;
						else if(t>1 && t<=22) t=(t&3)+146;
						else if(t>22) t=51-(t>>1);
						register int yoff=166-j;
						PIXEL(x+18+i,yoff)=t;
					}
			}
		}
		NumOk=0;
		for(i=0; i<=NUM_OPPONENT; i++){
			int nw=ManInform[i].nBuyWea;
			tri_scr_x_min=MAN_SCR_X[IX_SCR[i]];
			bar(tri_scr_x_min+15,5,tri_scr_x_min+85,60,0);
			bar(tri_scr_x_min,63,tri_scr_x_min+101,80,0);
			(ManInform[i].*ManInfBuy[CONTROL[i]])();
			ManInform[i].GetInfo();
			mod_sc=105;
			if(nw<SHOP_ARMOR) shape_view.copy(shape[nw+30]);
			else if(nw==SHOP_RENTGEN) shape_view.copy(shape[71]),mod_sc=95;
			else if(nw==SHOP_RADAR) shape_view.copy(shape[69]),mod_sc=95;
			else if(nw==SHOP_ARMOR) shape_view.copy(shape[72]),mod_sc=90;
			else if(nw==SHOP_LEVEL) shape_view.copy(shape[92]),mod_sc=90;
			else shape_view.copy(shape[79]);
			shape_view.turn_y(); shape_view.persp(); shape_view.scale();
			shape_view.Go(tri_scr_x_min+50,21);
			shape_view.DrawFullScr();
			if(ManInform[i].isOk) NumOk++;
		}
		if((mod_turn+=6)>=ANGLE_360) mod_turn-=ANGLE_360;
		delay(90); SWBUFF();
		if(key[iF10]) if(QuitGame()) { GAME_OVER=100; return; }
#if IS_DEBUG
		if(key[iESC]){ while(key[iESC]); break; }
#endif
	}while(NumOk<=NUM_OPPONENT);
	LoadWorld(NEXT_LEV);
	GAME_IS_GO=1;
	GAME_OVER=0;
}//TOMArEngine::Shop()

extern void StringRight(char far *str,int x,int y);

void TOMArEngine::GameOver(){
	LoadScreen(SCREEN_OVER);
	int i,nD[3]={-32000,-32000,-32000},place[3]={0,0,0},x,y;
	int x_place[3]={35,207,123},y_place[3]={73,54,16};
	char *bkg[3];
	for(i=0; i<3; i++) bkg[i]=new char[4000];
	for(i=0; i<=NUM_OPPONENT; i++) nD[i]=(Man[i]->nKill);
	for(i=0; i<3; i++){
		if(nD[i]>nD[0]) place[i]++;
		if(nD[i]>nD[1]) place[i]++;
		if(nD[i]>nD[2]) place[i]++;
	}
	for(i=0; i<3; i++){
		if(place[0]==place[1]){
			if(Man[0]->doh>=Man[1]->doh) place[0]++; else place[1]++;
		}if(place[1]==place[2]){
			if(Man[1]->doh>=Man[2]->doh) place[1]++; else place[2]++;
		}if(place[0]==place[2]){
			if(Man[0]->doh>=Man[2]->doh) place[0]++; else place[2]++;
		}
	}
	char far *ptr;
	for(i=0; i<3; i++){
		ptr=bkg[i];
		for(x=x_place[place[i]]; x<x_place[place[i]]+70; x++)
			for(y=y_place[place[i]]; y<y_place[place[i]]+50; y++)
				*ptr=PIXEL(x,y),ptr++;
	}
	mod_turn=0;
	for(int done=0; done<11; done++){
		for(i=0; i<=NUM_OPPONENT; i++){
			ptr=bkg[i];
			for(x=x_place[place[i]]; x<x_place[place[i]]+70; x++)
				for(y=y_place[place[i]]; y<y_place[place[i]]+50; y++)
					PIXEL(x,y)=*ptr,ptr++;
			delay(166);
			if(done&1) shape_view.copy(shape[1+(i<<3)]);
			else shape_view.copy(shape[2+(i<<3)]);
			shape_view.turn_y(); mod_sc=150; shape_view.scale();
			shape_view.Go(x_place[place[i]]+35,y_place[place[i]]+18);
			shape_view.DrawFullScr();
		}
		if((mod_turn+=30)>=ANGLE_360) mod_turn-=ANGLE_360;
		SWBUFF();
	}
	for(i=0; i<=NUM_OPPONENT; i++){
		StringCenter(NAME[i],x_place[place[i]]+50,y_place[place[i]]-15);
		ClrCharAt(3,x_place[place[i]]+21,y_place[place[i]]-7);
		int d_x=x_place[place[i]]+27;
		if(nD[i]>0){ String("+",d_x,y_place[place[i]]-7); d_x+=6; }
		itoa(nD[i],_n_,10); String(_n_,d_x,y_place[place[i]]-7);
		ltoa(Man[i]->doh,_n_,10); String(_n_,d_x,y_place[place[i]]-1);
		String("$",d_x-7,y_place[place[i]]-1);
	}
	SWBUFF();
	delay(333);
	for(i=0; i<3; i++) delete[] bkg[i];
	CreateOptionsFile();
	delete[] LightLev,pWall,pBurst,pFacet,SIN_TABLE,tan_table,
		inv_tan_table,y_step,x_step,cos_table,inv_cos_table,inv_sin_table;
	char *Buf=new char[64000U];
	if(Buf) for(i=200; i>-550; i--){
		int y=i;
		move64K(Buf,GrBuff);
		RED_CHARS;
		StringCenter("OMAr",160,y);
		StringCenter("OMAr",160,y+1); y+=25;
		WHITE_CHARS;
		StringRight("Программист",153,y); String("( t e п A N   П У r K",167,y); y+=8;
		StringRight("Художник",153,y); String("он же",167,y); y+=8;
		StringRight("В главной роли",153,y); String("я же",167,y); y+=8;
		y+=15;
		StringRight("Ассистенты режиссера",153,y); y+=8;
		StringRight("по полезным советам:",153,y); String("Игорь Поэтов",167,y); y+=8;
		String("Надежда Пурк",167,y);
		y+=15;
		StringRight("Тестеры",153,y);
		String("Игорь Поэтов",167,y); y+=8;
		String("Надежда Пурк",167,y); y+=8;
		String("Ромаs Злобин",167,y); y+=8;
		String("МаХ Лачкоff",167,y); y+=8;
		String("Сергей Злобин",167,y); y+=8;
		String("Алексей Лачков",167,y); y+=8;
		String("Татьяна Антоненко",167,y);
		y+=20;
		StringRight("Сюжет,",153,y); y+=8;
		StringRight("Дизайн,",153,y); y+=8;
		StringRight("Анимация,",153,y); y+=8;
		StringRight("Трехмерное моделирование,",153,y); y+=8;
		StringRight("Автор сценария,",153,y); y+=8;
		StringRight("Режиссер и продюссер",153,y); String("Степан Пурк",167,y);
		y+=20;
		StringCenter("На стенах повсюду нарисована  NадежDа ПУRк",160,y); y+=20;
		StringRight("Владелец всех прав",153,y); y+=8;
		StringRight("на игру OMAr",153,y); String("Степан Пурк",167,y); y+=15;
		StringRight("Звуковые эффекты",153,y); y+=8;
		StringRight("не сделал",153,y); String("тоже Степан!",167,y); y+=15;
		StringCenter("(не умею программировать SFX под DOS)",160,y); y+=10;
		y+=50;
		StringCenter("Если у вас возникли идеи, которыми вы хотите",160,y); y+=10;
		StringCenter("поделиться со мной, напишите письмо",160,y); y+=30;
		RED_CHARS;
		StringCenter("e-mail:  PURK@BEREG.IRKUTSK.RU",160,y);
		WHITE_CHARS;
		y+=66;
		StringCenter("Большое спасибо вам за то,",160,y); y+=8;
		StringCenter("что прочитали все титры",160,y);
		y+=30;
		MAGENTA_CHARS;
		StringCenter("Братск, 1998 год",160,y);
		SWBUFF();
		delay(133);
		move64K(GrBuff,Buf);
		if(key[iESC] || key[iF10] || key[iENTER] || key[iSPACE]) break;
		delete[] Buf;
	}else{
		while(!key[iESC] && !key[iF10] && !key[iENTER] & !key[iSPACE]);
	}
	delete[] GrBuff,pFon6,_n_;
	mode(3);
}

void TOMArEngine::Run(){
	int i,j;
	randomize();
	NEXT_LEV=random(NUM_LEVELS);
	Logo();
	MainMenu();
	nosound();
	while(GAME_OVER<99){
#if IS_DEBUG
		Msg("",0); // Стереть старую отладочную информацию
#endif
		for(i=0; i<=NUM_OBJ; i++) if(Obj[i]->Move()==DELETE) DelObj(i--);
		SetArmPosition(); ClearStrings(); InformStr();
		if(GOLD_LEFT<1){
			// когда собрали все золото,
			// не будем заканчивать уровень,
			// лучше заcыпем его песком
			//GAME_OVER=1;
			LEVEL_TYPE=0;
		}
		if(TIME_LEFT<17) GAME_OVER=2;
		if(IS_MONSTERS && !NUM_MONS && !NUM_INCUB && !NUM_OPPONENT) GAME_OVER=3;//Убили всех монстров
		for(i=0; i<=NUM_OPPONENT; i++){
			Man[i]->View();
			if(KILLED[i]>=LEV_KILL) //Если на этом уровне кого-то убивали
				GAME_OVER=10+i;      //слишком много, уровень пройден
			if((Man[i]->nKill<=-MAX_KILL) || (Man[i]->nKill>=MAX_KILL))
				GAME_OVER=99; //Если кто-либо набрал MAX_KILL очков, игра кончается
		}
		while(sw_buff<1);
		____CA(); SWBUFF(); sw_buff--; CADR_NUMBER++;
		if(!(tic&511) && ITEMS_LEFT<Options.items) NewItem(),tic++;
		if(0==LEVEL_TYPE && 0==(tic&31)) AddSand(),tic++;
		if((items_turn_ang+=15)>ANGLE_360) items_turn_ang=0;
#if IS_DEBUG
		if(key[iEND]) Chats();
		if(key[iF5]) Inform();
		if(key[iF4]) ShowMsg();
#endif
		if(key[iF9]) key[iF9]=0,LoadGame(SAVE_FILE);
		else if(key[iF6]) key[iF6]=0,SaveGame(SAVE_FILE);
		else if(key[iF7]) key[iF7]=0,LoadGame(EXIT_FILE_SAVE);
		else if(key[iF8]) key[iF8]=0,LoadGame(PREV_FILE_NAME);
		else if(key[iF1]) Help();
		else if(key[iF2]) SaveLoadName(1);
		else if(key[iF3]) SaveLoadName(0);
		if(key[iESC]) MainMenu(0);
		if(key[iF10]) if(QuitGame()) GAME_OVER=100;
		if(GAME_OVER){
			if(GAME_OVER<100){
				if(LEVELS_PASS==Options.levels_to_go){
					// проверить, что счет неравный,
					// если очки у двух или трех игроков совпадают,
					// продолжаем играть
					int ok=1;
					for(i=0; i<=NUM_OPPONENT; i++)
						for(j=0; j<=NUM_OPPONENT; j++)
							if(i!=j && Man[i]->nKill==Man[j]->nKill)
								ok=0;
					if(ok==0) GAME_OVER=0;
					else GAME_OVER=99;
				}else{
					Shop();
					LEVELS_PASS++;
				}
			}
		}
	}
	if(GAME_OVER==99) GameOver();
	else{ FreeMem(); mode(3); }
}
