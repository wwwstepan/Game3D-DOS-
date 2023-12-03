#include <dos.h>
#include <alloc.h>
#include <stdlib.h>
#include "c\3d.h"
#include "c\monster.h"
#include "c\engine.h"

// void TOMArEngine::Weapons(){
// void TOMArEngine::Controls(){
// void TOMArEngine::ManHealth(){
// void TOMArEngine::ManScreen(){
// void TOMArEngine::PrintOptions(int menu){
// void TOMArEngine::MainMenu(int first){

char *Press_ESC="ç†¶¨®‚• ESC";

void TOMArEngine::Weapons(){
	UNS i; int nw=1,turn_wea[NUM_WEAP];
	for(i=0; i<NUM_WEAP; i++) turn_wea[i]=ANGLE_270;
	_n_[10]=0;
	while(key[iESC] || key[iENTER]);
	while(!key[iESC] && !key[iENTER]){
		Clear64K(GrBuff);
		for(i=1; i<NUM_WEAP; i++){
			shape_view.copy(shape[i+30]); mod_turn=turn_wea[i];
			shape_view.turn_y(); shape_view.persp();
			mod_sc=110; shape_view.scale();
			shape_view.Go(32+LIC_WEA[i]*64,i*16); shape_view.DrawFullScr();
		}
		for(i=0; i<10; i++) _n_[i]=NAME[0][i]; StringCenter(_n_,32,2);
		for(i=0; i<10; i++) _n_[i]=NAME[1][i]; StringCenter(_n_,96,2);
		for(i=0; i<10; i++) _n_[i]=NAME[2][i]; StringCenter(_n_,160,2);
		StringCenter("é°È••",224,2); StringCenter("á†Ø‡•È•≠Æ",288,2);
		StringCenter(WEAPS_NAME[nw],160,193);
		if(key[iUP] && nw>1) nw--,key[iUP]=0;
		if(key[iDOWN] && nw<NUM_WEAP-1) nw++,key[iDOWN]=0;
		if(key[iLEFT] && LIC_WEA[nw]>0) LIC_WEA[nw]--,key[iLEFT]=0;
		if(key[iRIGHT] && LIC_WEA[nw]<4) LIC_WEA[nw]++,key[iRIGHT]=0;
		if((turn_wea[nw]-=7)<0) turn_wea[nw]+=ANGLE_360;
		delay(20);
		SWBUFF();
	}
	while(key[iESC] || key[iENTER]);
	LoadScreen(SCREEN_MENU); SWBUFF();
}

void TOMArEngine::Controls(){
	int i,j; GrPtr=GrArea;
	while(key[iESC] || key[iENTER]);
	while(!key[iESC] && !key[iENTER]){
		for(i=30; i<290; i++)
			for(j=35; j<84; j++)
				PIXEL(i,j)=rand()&3;
		String("ä´†¢®Ë® „Ø‡†¢´•≠®Ô",106,38);
		for(i=0; i<3; i++){
			String(NAME[i],40,49+i*7); String(STR_DTA[1+CONTROL[i]],142,49+i*7);
		}
		String(Press_ESC,127,76);
		while(!key[iESC] && !key[iENTER] && !key[iUP] && !key[iDOWN] && !key[iLEFT] && !key[iRIGHT]);
		if(key[iUP]){
			swap(CONTROL[0],CONTROL[1]); delay(133);
		}else if(key[iDOWN]){
			swap(CONTROL[1],CONTROL[2]); delay(133);
		}else if(key[iLEFT]){
			swap(CONTROL[0],CONTROL[1],CONTROL[2]); delay(150);
		}else if(key[iRIGHT]){
			swap(CONTROL[2],CONTROL[1],CONTROL[0]); delay(150);
		}
	}
	while(key[iESC] || key[iENTER]);
	GrPtr=GrBuff; SWBUFF();
}

void TOMArEngine::ManHealth(){
	int i,j,t=0; GrPtr=GrArea;
	for(i=30; i<290; i++)
		for(j=35; j<84; j++)
			PIXEL(i,j)=rand()&3;
	String("ÇÎ≠Æ·´®¢Æ·‚Ï ®£‡Æ™Æ¢",100,38);
	String(Press_ESC,127,76);
	while(key[iESC] || key[iENTER]);
	while(!key[iESC] && !key[iENTER]){
		bar(89,48,226,69,0);
		for(i=0; i<3; i++){
			String(NAME[i],98,49+i*7);
			String(MAN_HLTH[i]/1.2795,199,49+i*7);
		}
		ClrCharAt(15,90,49+t*7); ClrCharAt(16,220,49+t*7);
		while(!key[iESC] && !key[iENTER] && !key[iUP] && !key[iDOWN] && !key[iLEFT] && !key[iRIGHT]);
		if(key[iUP]){ if(--t<0) t=2; delay(150); }
		if(key[iDOWN]){ if(++t>2) t=0; delay(150); }
		if(key[iLEFT]){ if(--MAN_HLTH[t]<7) MAN_HLTH[t]=7; delay(99); }
		if(key[iRIGHT]){ if(++MAN_HLTH[t]>128) MAN_HLTH[t]=128; delay(99); }
	}
	while(key[iESC] || key[iENTER]);
	GrPtr=GrBuff; SWBUFF();
}

void TOMArEngine::ManScreen(){
	int i,faze=0,turn_man_scr[3],dir[3];
	UNS u;
	for(i=0; i<3; i++)
		turn_man_scr[i]=ANGLE_90,dir[i]=1;
	for(u=23040; u<64000U; u++) GrBuff[u]=0;
	String("ù™‡†≠Î Æ°ßÆ‡†",121,74);
	String(Press_ESC,127,185);
	while(key[iESC] || key[iENTER]);
	while(!key[iESC] && !key[iENTER]){
		for(u=25600; u<56960U; u++) GrBuff[u]=0;
		if(faze++>3) faze=0;
		for(i=0; i<3; i++){
			if(dir[i]){
				if(turn_man_scr[i]<ANGLE_270) turn_man_scr[i]+=random(15);
				else dir[i]=!dir[i];
			}else{
				if(turn_man_scr[i]>ANGLE_90) turn_man_scr[i]-=random(15);
				else dir[i]=!dir[i];
			}
			StringCenter(NAME[i],MAN_SCR_X[IX_SCR[i]]+50,85);
			shape_view.copy(shape[faze+(i<<3)]);
			mod_turn=turn_man_scr[i];
			shape_view.turn_y(); shape_view.persp();
			mod_sc=130; shape_view.scale();
			shape_view.Go(MAN_SCR_X[IX_SCR[i]]+ANGLE_30,120);
			shape_view.DrawFullScr();
		}
		SWBUFF();
		char ind[3];
		for(i=0; i<3; i++)
			ind[i]=i;
		if(IX_SCR[ind[0]]>IX_SCR[ind[1]])
			swap(ind[0],ind[1]);
		if(IX_SCR[ind[1]]>IX_SCR[ind[2]])
			swap(ind[1],ind[2]);
		if(IX_SCR[ind[0]]>IX_SCR[ind[1]])
			swap(ind[0],ind[1]);
		if(key[iLEFT])
			swap(IX_SCR[ind[0]],IX_SCR[ind[1]]);
		else if(key[iRIGHT])
			swap(IX_SCR[ind[1]],IX_SCR[ind[2]]);
		else if(key[iUP])
			swap(IX_SCR[0],IX_SCR[1],IX_SCR[2]);
		else if(key[iDOWN])
			swap(IX_SCR[2],IX_SCR[1],IX_SCR[0]);
		while((key[iUP] || key[iDOWN] || key[iLEFT] || key[iRIGHT]));// && !key[iESC] && !key[iENTER]);
		delay(50);
	}
	while(key[iESC] || key[iENTER]);
	for(i=0; i<=NUM_OPPONENT; i++)
		if(Man[i])
			Man[i]->SCR_X_MIN=MAN_SCR_X[IX_SCR[Man[i]->number]];
	LoadScreen(SCREEN_MENU); SWBUFF();
}

void TOMArEngine::PrintOptions(int menu){
	int oy;

	bar(42,Y_OPTIONS(OP_NMAN)-3,270,Y_OPTIONS(OP_QUIT)+8,0);
	oy=Y_OPTIONS(OP_NMAN);
	String("óàëãé àÉêéäéÇ",60,oy); String(Options.n_man,220,oy);

	oy=Y_OPTIONS(OP_GAMTYPE);
	String("íàè àÉêõ:",60,oy); String(GAMETYPES[GAME_TYPE],135,oy);

//	oy=Y_OPTIONS(OP_GOLD);
//	String("äéãàóÖëíÇé áéãéíÄ",60,oy); String(Options.gold,220,oy);

	oy=Y_OPTIONS(OP_ITEMS);
	String("äéãàóÖëíÇé èêàáéÇ",60,oy); String(Options.items,220,oy);

	oy=Y_OPTIONS(OP_TIME);
	String("ÇêÖåü (Ç åàçìíÄï)",60,oy); String(1+Options.time_limit/1092,220,oy);

	oy=Y_OPTIONS(OP_LEVELS);
	String("äéãàóÖëíÇé ìêéÇçÖâ",60,oy); String(Options.levels_to_go,220,oy);

	oy=Y_OPTIONS(OP_GAMMA);
	String("üêäéëíú èÄãàíêõ (%)",60,oy); String(Options.gamma,220,oy);

	oy=Y_OPTIONS(OP_1PLAYER);
	String("1-© àÉêéä:",60,oy); String(NAME[0],120,oy);
	String("$",220,oy); String(MAN_PRICE[0],226,oy);

	oy=Y_OPTIONS(OP_2PLAYER);
	String("2-© àÉêéä:",60,oy); String(NAME[1],120,oy);
	String("$",220,oy); String(MAN_PRICE[1],226,oy);

	oy=Y_OPTIONS(OP_3PLAYER);
	String("3-© àÉêéä:",60,oy); String(NAME[2],120,oy);
	String("$",220,oy); String(MAN_PRICE[2],226,oy);

	oy=Y_OPTIONS(OP_SAVE);
	String("áÄèàëú àÉêõ",60,oy); String(SAVE_FILE,190,oy);
	String("ÇõçéëãàÇéëíú àÉêéäéÇ",100,Y_OPTIONS(OP_HLTH));
	String("êÄëèéãéÜÖçàÖ ùäêÄçéÇ",100,Y_OPTIONS(OP_SCREENS));
	String("ìèêÄÇãÖçàÖ",130,Y_OPTIONS(OP_CONTROL));
	String("ÇééêìÜÖçàÖ àÉêéäéÇ",106,Y_OPTIONS(OP_WEAPS));
	String("çÄóÄíú àÉêì",127,Y_OPTIONS(OP_NEWGAME));
	String("ÇÖêçìíúëü ä àÉêÖ",112,Y_OPTIONS(OP_BACK));
	String("Çõâíà àá àÉêõ",121,Y_OPTIONS(OP_QUIT));

	oy=Y_OPTIONS(menu); ClrCharAt(15,44,oy); ClrCharAt(16,264,oy);
	SWBUFF();
}

void TOMArEngine::MainMenu(int first){
	int i,menu,t,gamma_change,Exit=0; GAME_IS_GO=0;
	LoadScreen(SCREEN_MENU); SWBUFF();
	for(i=0; SAVE_FILE[i]!='.' && SAVE_FILE[i]!=0; i++);
	SAVE_FILE[i++]=0;
	menu=first?OP_NEWGAME:OP_BACK;
	PrintOptions(menu);
	while(key[iESC]);
	while(!Exit){
		gamma_change=0;
		switch(menu){
		case OP_NMAN: InpNumber(222,Y_OPTIONS(OP_NMAN)-1,1,1,1,3,Options.n_man); break;
		case OP_GAMTYPE:
			GrPtr=GrArea; BLUE_CHARS;
			do{
				bar(135,Y_OPTIONS(OP_GAMTYPE),260,Y_OPTIONS(OP_GAMTYPE)+6,0);
				String(GAMETYPES[GAME_TYPE],135,Y_OPTIONS(OP_GAMTYPE));
				key_pressed(); delay(20);
				if(key[iLEFT]){ if(GAME_TYPE>0) GAME_TYPE--; }
				else if(key[iRIGHT]){ if(GAME_TYPE<2) GAME_TYPE++; }
			}while(!key[iUP] && !key[iDOWN] && !key[iF10] && !key[iESC]);
			GrPtr=GrBuff; GREEN_CHARS;
			break;
//		case OP_GOLD: InpNumber(222,Y_OPTIONS(OP_GOLD)-1,4,10,50,3000,Options.gold); break;
		case OP_ITEMS: InpNumber(222,Y_OPTIONS(OP_ITEMS)-1,2,1,0,50,Options.items); break;
		case OP_TIME: t=1+Options.time_limit/1092;
			InpNumber(222,Y_OPTIONS(OP_TIME)-1,2,1,1,60,t);
			Options.time_limit=t*1092U-1; break;
		case OP_LEVELS:
			InpNumber(222,Y_OPTIONS(OP_LEVELS)-1,2,1,LEVELS_PASS,99,Options.levels_to_go);
			break;
		case OP_GAMMA:
			gamma_change=InpNumber(222,Y_OPTIONS(OP_GAMMA)-1,3,5,25,250,Options.gamma);
			break;
		case OP_SAVE: InpText(192,Y_OPTIONS(OP_SAVE)-1,9,SAVE_FILE); break;
		case OP_1PLAYER: InpMan(122,Y_OPTIONS(OP_1PLAYER)-1,0); break;
		case OP_2PLAYER: InpMan(122,Y_OPTIONS(OP_2PLAYER)-1,1); break;
		case OP_3PLAYER: InpMan(122,Y_OPTIONS(OP_3PLAYER)-1,2); break;
		default: delay(150);
		}
		while(!key[iUP] && !key[iDOWN] && !key[iF10] && !key[iESC] && !key[iENTER] && !key[iF5]);
		if(key[iESC] && !first) Exit=1;
		if(key[iF10]) if(QuitGame()) Quit();
		if(key[iENTER]){
			PrintOptions(menu);
			if(menu==OP_WEAPS) Weapons();
			else if(menu==OP_SCREENS) ManScreen();
			else if(menu==OP_CONTROL) Controls();
			else if(menu==OP_HLTH) ManHealth();
			else if(menu==OP_NEWGAME){
				int ng=first?1:RealyNewGame();
				if(ng){
					if(!first) SaveGame(EXIT_FILE_SAVE);
					for(i=0; i<3; i++) ManInform[i].New(i);
					LEVELS_PASS=1;
					LoadWorld();
					break;
				}
			}else if(menu==OP_BACK && !first) break;
			else if(menu==OP_QUIT){
				while(key[iENTER]);
				if(QuitGame()){
					if(!first) SaveGame(EXIT_FILE_SAVE);
					Quit();
				}
			}else menu++;
		}
		//////
		if(key[iF5]) MapEditor();
		//////
		if(key[iUP]) menu--; if(key[iDOWN]) menu++;
		if(menu<0) menu=OP_QUIT; else if(menu>OP_QUIT) menu=0;
		if(gamma_change)
			LoadScreen(SCREEN_MENU);
		PrintOptions(menu);
	}
	AddSavExt(SAVE_FILE);
	CADR_NUMBER=0L;
	while(key[iESC]);
	GAME_IS_GO=1;
}