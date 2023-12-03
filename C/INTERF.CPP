#include <io.h>
#include <fcntl.h>
#include <dos.h>
#include <alloc.h>
#include <stdlib.h>
#include "c\3d.h"
#include "c\monster.h"
#include "c\engine.h"

// void TOMArEngine::AddSand(){
// void TOMArEngine::Chats(){
// void Msg(int n,int add){
// void Msg(char *msg,int add){
// void TOMArEngine::ShowMsg(){
// void TOMArEngine::Inform(){
// void TOMArEngine::InpText(int ix,int iy,int nch,char *s){
// int TOMArEngine::InpNumber(int ix,int iy,int nch,int step,int min,int max,int &Num,int scroll){
// void TOMArEngine::InpMan(int ix,int iy,int nman){
// int TOMArEngine::InputStr(char *title,char *s,int max_len){
// int TOMArEngine::QuitGame(){
// void TOMArEngine::Help(){
// int TOMArEngine::RealyNewGame(){
// void TOMArEngine::CreateOptionsFile(){
// void TOMArEngine::LoadOptions(){
// int TOMArEngine::CheckEXE(){

char key_chars[4][64]={{
	'±','±','1','2','3','4','5','6','7','8','9','0','-','=','±','±',
	'q','w','e','r','t','y','u','i','o','p','[',']','±','±','a','s',
	'd','f','g','h','j','k','l',';','\'','`','±','\\','z','x','c','v',
	'b','n','m',',','.','/','±','±','±',' ','±','±','±','±','±','±'
},{
	'±','±','!','@','#','$','%','^','&','*','(',')','_','+','±','±',
	'Q','W','E','R','T','Y','U','I','O','P','{','}','±','±','A','S',
	'D','F','G','H','J','K','L',':','\"','~','±','|','Z','X','C','V',
	'B','N','M','<','>','?','±','±','±',' ','±','±','±','±','±','±'
},{
	'±','±','1','2','3','4','5','6','7','8','9','0','-','=','±','±',
	'©','æ','ã','ª','¥','­','£','è','é','§','å','ê','±','±','ä','ë',
	'¢',' ','¯','à','®','«','¤','¦','í','ñ','±','|','ï','ç','á','¬',
	'¨','â','ì','¡','î','ñ','±','±','±',' ','±','±','±','±','±','±'
},{
	'±','±','!','@','#','$','%','^','&','*','(',')','_','+','±','±',
	'‰','–','“','Š','…','','ƒ','˜','™','‡','•','š','±','±','”','›',
	'‚','€','','','','‹','„','†','','ğ','±','\\','Ÿ','—','‘','Œ',
	'ˆ','’','œ','','','ğ','±','±','±',' ','±','±','±','±','±','±'
}};

void TOMArEngine::AddSand(){
	register int i; int j=100,X,Y,Obj_in_way;
	do{
		X=random(WORLD_COL-2); Y=random(WORLD_ROW-2); X++; Y++;
		Obj_in_way=0;
		if(!world[X][Y]){
			for(i=0; i<=NUM_OBJ && !Obj_in_way; i++)
				if((((Obj[i]->x)>>6)==X) && (((Obj[i]->y)>>6)==Y)){
					Obj_in_way=1; break;
				}
			if(!Obj_in_way) break;
		}
	}while(--j>0);
	if(j){
		int type_sand=(rand()&3);
		world[X][Y]=23+type_sand;
		if(0==(rand()&31) && GOLD_LEFT<9999){
			x_obj=(X<<6)+16+(rand()&31); y_obj=(Y<<6)+16+(rand()&31);
			t_obj=10+random(101)+type_sand*30;
			if(GOLD_LEFT+t_obj>9999) t_obj=9999-GOLD_LEFT;
			if(AddObj(new TGold)) GOLD_LEFT+=t_obj;
		}
	}
}

#if IS_DEBUG
void TOMArEngine::Chats(){
	GAME_IS_GO=0;
	if(key[iTILDA]){
		GrPtr=GrArea;
		String("Š®¤ë",100,20);
		String("1 - “†ˆ…",70,30);
		String("2 - Šˆ‘’€‹‹,…’ƒ…,€„€",70,38);
		String("3 - Œƒ ˆ",70,46);
		String("4 - ‚…ŒŸ + 10 Œˆ.",70,54);
		String("5 - “€’œ ,‡„‚œ…=100",70,62);
		String("6 - —ˆ‘’ˆ’œ “‚…œ",70,70);
		String("7 - ’€’œ “ ‚‘…• “†ˆ…",70,78);
		String("8 - ‚…ŒŸ = 3 ‘…Š.",70,86);
		String("9 - „. “‚…œ",70,94);
		String("0 - „€‚ˆ’œ Œ‘’€",70,102);
		String("Obj - ‚á¥ ®¡ê¥ªâë",70,110);
		while(!key[i1] && !key[i2] && !key[i3] &&
			!key[i4] && !key[i5] && !key[i6] && !key[i7]
			&& !key[i8] && !key[i9] && !key[i0] && !key[iO]);
		SWBUFF(); GrPtr=GrBuff;
	}
	int i,j;
	if(key[i1]){
		for(i=0; i<=NUM_OPPONENT; i++){
			for(j=1; j<NUM_WEAP; j++)
				Man[i]->ammo[j]=MAX_AMMO[j]-1;
			Man[i]->Info(26);
		}
	}if(key[i2]){
		for(i=0; i<=NUM_OPPONENT; i++){
			Man[i]->radar=1;
			Man[i]->invis=25344;
			Man[i]->slip=500;
			Man[i]->rent=2000;
			Man[i]->Info(26);
		}
	}if(key[i3]){
		for(i=0; i<=NUM_OPPONENT; i++){
			Man[i]->armor=6336; Man[i]->Info(26);
		}
	}if(key[i4]){
		if(TIME_LEFT<=65535U-10920) TIME_LEFT+=10920;
		else if(TIME_LEFT>65535U-100 && TIME_LEFT<65535U-15){ TIME_LEFT=50; while(key[i4]); }
		else TIME_LEFT=65535U;
		for(i=0; i<=NUM_OPPONENT; i++){
			//Man[i]->cash=300;
			Man[i]->Info(26);
		}
	}if(key[i5]){
		for(i=0; i<=NUM_OPPONENT; i++){
			Man[i]->armor=Man[i]->slip=Man[i]->rent=Man[i]->radar=Man[i]->invis=0;
			Man[i]->health=MAX_HEALTH; Man[i]->Info(26);
		}
	}if(key[i6]){
		for(i=0; i<WORLD_COL; i++)
			for(j=0; j<WORLD_ROW; j++)
				if(i>0 && j>0 && i<22 && j<22) world[i][j]=0;
				else world[i][j]=random(20)+2;
		for(i=0; i<=NUM_OPPONENT; i++){
			Man[i]->health=MAX_HEALTH;
			Man[i]->x=400; Man[i]->y=600-Man[i]->number*128;
			Man[i]->angle=ANGLE_90; Man[i]->Info(26);
		}
	}if(key[i7]){
		for(i=0; i<=NUM_OPPONENT; i++){
			for(j=1; j<NUM_WEAP; j++)
				Man[i]->ammo[j]=0;
			Man[i]->nWeap=0; Man[i]->Info(26);
		}
	}if(key[i8]){
		TIME_LEFT=70;
		for(i=0; i<=NUM_OPPONENT; i++){
			Man[i]->cash=300;
			Man[i]->Info(26);
		}
	}if(key[i9]){
		LoadWorld();
	}if(key[i0]){
		x_obj=0; n_obj=MIN_MONSTER_TYPE+(rand()%MONST_TYPES);
		AddObj(new TMonster);
		delay(333);
	}if(key[iO]){
		Msg("Obj: ",0);
		for(int i=0; i<=NUM_OBJ; i++){
			int its=Obj[i]->Its();
			Msg(its); Msg(",");
			if((i&15)==15) Msg("\xFF");
		}
		ShowMsg();
//		SWBUFF();
		while(key[iO]);
	}
	GAME_IS_GO=1;
}

char sMsg[MSG_LEN];
int iMsg=0,xMsg=0,yMsg=0;

void Msg(int n,int add){
	itoa(n,_n_,10);
	Msg(_n_,add); Msg(" ");
}

void Msg(char *msg,int add){
	if(!add) iMsg=xMsg=yMsg=0;
	int i,j;
	for(i=iMsg,j=0; i<(MSG_LEN-5) && msg[j]; i++,j++,xMsg++,iMsg++){
		if(xMsg>50){ sMsg[i++]='\xFF'; iMsg++; yMsg++; xMsg=0; }
		char c=msg[j];
		if(c=='\n' || c=='\r' || c=='\xFF'){ c='\xFF'; yMsg++; xMsg=0; }
		sMsg[i]=c;
	}
	sMsg[i]=0;
}

void TOMArEngine::ShowMsg(){
	GAME_IS_GO=0; GrPtr=GrArea;
	UNS u2=yMsg*6*320+640;
	for(UNS u=0; u<u2; u++){
		GrPtr[u]&=15;
		if(GrPtr[u]>13) GrPtr[u]=13;
		GrPtr[u]+=160;
	}
	String(sMsg,1,1);
	while(key[iF4]); SWBUFF(); GrPtr=GrBuff; GAME_IS_GO=1;
}

void TOMArEngine::Inform(){
	GAME_IS_GO=0; GrPtr=GrArea;
	String("‘¨áâ¥¬­ ï ¨­ä®à¬ æ¨ï",100,20);
	String("¡ê¥ªâ®¢ ¢ ¨£à¥",70,30);
	String(NUM_OBJ,195,30);
	String("Š®«¨ç¥áâ¢® ¯à¨§®¢",70,36);
	String(ITEMS_LEFT,195,36);
	String("‘¢®¡®¤­ ï ¯ ¬ïâì",70,42);
	ltoa(farcoreleft(),_n_,10); String(_n_,195,42);
	String("®¬¥à ª ¤à ",70,48);
	ultoa(CADR_NUMBER,_n_,10); String(_n_,195,48);
	String("“¬¥à«¨/“¡¨«¨ (­  íâ®¬ ãà®¢­¥):",70,54);
	String(NAME[0],70,60); String(KILLED[0],180,60); String("/",220,60); String(KILL[0],227,60);
	String(NAME[1],70,66); String(KILLED[1],180,66); String("/",220,66); String(KILL[1],227,66);
	String(NAME[2],70,72); String(KILLED[2],180,72); String("/",220,72); String(KILL[2],227,72);
	while(key[iF5]); SWBUFF(); GrPtr=GrBuff; GAME_IS_GO=1;
}
#endif
void TOMArEngine::InpText(int ix,int iy,int nch,char *s){
	int k,indx_chr,str_len,ix2=ix+nch*6+4,iy2=iy+6;
	for(str_len=0; s[str_len]; str_len++);
	GrPtr=GrArea;
	do{
		bar(ix-2,iy,ix2,iy2,0);
		String(s,ix+2,iy+1);
		String("Û",ix+2+str_len*6,iy+1);
		k=key_pressed();
		delay(20);
		if(k<64 && k>0){
			indx_chr=0;
			if(k==iBSPACE && str_len>0) s[str_len--]=0;
			if(key[iRSHIFT] || key[iLSHIFT]) indx_chr=1;
			else{ if(key[iALT]) indx_chr+=2; if(key[iCTRL]) indx_chr+=3; }
			char c=key_chars[indx_chr][k];
			if(c!='±' && str_len<nch-1) s[str_len++]=c,s[str_len]=0;
		}
	}while(!key[iENTER] && !key[iUP] && !key[iDOWN] && !key[iF10] && !key[iESC]);
	s[str_len]=0; GrPtr=GrBuff;
}

int TOMArEngine::InpNumber(int ix,int iy,int nch,int step,int min,int max,int &Num,int scroll){
	int oldNum=Num,ix2=ix+nch*6+3,iy2=iy+6;
	GrPtr=GrArea;
	do{
		bar(ix-2,iy,ix2,iy2,0); String(Num,ix+2,iy+1);
		key_pressed();
		delay(20);
		if(key[iLEFT]) Num-=step; else if(key[iRIGHT]) Num+=step;
		if(Num<min) Num=scroll?max:min;
		else if(Num>max) Num=scroll?min:max;
	}while(!key[iUP] && !key[iDOWN] && !key[iF10] && !key[iESC]);
	GrPtr=GrBuff;
	return Num!=oldNum;
}

void TOMArEngine::InpMan(int ix,int iy,int nman){
	char *s=NAME[nman]; int pr=MAN_PRICE[nman],nch=16;
	int k,indx_chr,str_len,iy2=iy+6;
	for(str_len=0; s[str_len]; str_len++);
	GrPtr=GrArea;
	do{
		bar(ix-2,iy,260,iy2,0);
		String(s,ix+2,iy+1);
		String("Û",ix+2+str_len*6,iy+1);
		String("$",ix+100,iy+1);
		String(pr,ix+106,iy+1);
		k=key_pressed();
		delay(20);
		if(key[iLEFT]) pr-=1; else if(key[iRIGHT]) pr+=1;
		if(pr<100) pr=100; else if(pr>500) pr=500;
		if(k<64 && k>0){
			indx_chr=0;
			if(k==iBSPACE && str_len>0) s[str_len--]=0;
			if(key[iRSHIFT] || key[iLSHIFT]) indx_chr=1;
			else{ if(key[iALT]) indx_chr+=2; if(key[iCTRL]) indx_chr+=3; }
			char c=key_chars[indx_chr][k];
			if(c!='±' && str_len<nch-1) s[str_len++]=c,s[str_len]=0;
		}
	}while(!key[iUP] && !key[iDOWN] && !key[iF10] && !key[iESC]);
	MAN_PRICE[nman]=pr;
	for(k=0; k<16; k++)
		if(s[k]==',')
			s[k]=';';
	s[str_len]=0; GrPtr=GrBuff;
}

int TOMArEngine::InputStr(char *title,char *s,int max_len){
	int i,j,g,answer=1,len_title,len2t;
	if(GAME_IS_GO) GAME_IS_GO=0,g=1;
	GrPtr=GrArea;
	for(i=0; i<50 && title[i]; i++);
	title[i]=0; len_title=i; len2t=len_title*3;
	for(i=150-len2t; i<=170+len2t; i++)
		for(j=71; j<=105; j++)
			PIXEL(i,j)=(i*j)&7+128;
	String(title,160-len2t,75);
	max_len++;
	while(!key[iENTER]){
		InpText(160-max_len*3,85,max_len,s);
		if(key[iESC]){ answer=0; break; }
	}
	while(key[iESC] || key[iENTER]);
	GrPtr=GrBuff; SWBUFF();
	if(g) GAME_IS_GO=1;
	return answer;
}

int TOMArEngine::QuitGame(){
	int i,j,g=0,answer;
	if(GAME_IS_GO) GAME_IS_GO=0,g=1;
	GrPtr=GrArea;
	for(i=119; i<=201; i++)
		for(j=71; j<=105; j++)
			PIXEL(i,j)=(i*j)&7+64;
	String("EXIT TO DOS?",124,86);
	while(1){
		if(key[iN] || key[iESC]){ answer=0; break; }
		else if(key[iY] || key[iENTER]){ answer=1; break; }
	}
	while(key[iY] || key[iN] || key[iENTER] || key[iESC]);
	if(answer) CreateOptionsFile();
	GrPtr=GrBuff; SWBUFF(); if(g) GAME_IS_GO=1; return answer;
}

void TOMArEngine::Help(){
	int i,j,g=0;
	if(GAME_IS_GO) GAME_IS_GO=0,g=1;
	GrPtr=GrArea;
	for(i=60; i<=260; i++)
		for(j=50; j<=150; j++)
			PIXEL(i,j)=(((i*j)&63)>>3)+96;
	String("OMAr",148,60);
	String("by",154,70);
	String("Altpapier & Altmetall",97,80);
	String("Producer, artist & programmer",73,90);
	String("Step T. Purk",124,105);
	String("1997",148,125);
	String("­ ¦¬¨ ENTER",127,144);
	while(!key[iENTER]); key[iENTER]=0;
	for(i=60; i<=260; i++)
		for(j=50; j<=150; j++)
			PIXEL(i,j)=(((i*j)&31)>>2)+96;
	String("F1  Œ™œ",80,70);
	String("F2  ‘•€ˆ’œ ˆƒ“",80,78);
	String("F3  ‡€ƒ“‡ˆ’œ ˆƒ“",80,86);
	String("F6  ›‘’€Ÿ ‡€ˆ‘œ",80,94);
	String("F9  ›‘’€Ÿ ‡€ƒ“‡Š€",80,102);
	String("F7  ‡€ƒ“‡ˆ’œ ‘‹…„ ˆƒ“",80,110);
	String("F8  ‡€ƒ“‡ˆ’œ …„›„“™“ ˆƒ“",80,118);
	String("F10 ‚›•„",80,126);
	String("­ ¦¬¨ ENTER",127,144);
	while(!key[iENTER]); key[iENTER]=0;
	GrPtr=GrBuff; SWBUFF(); if(g) GAME_IS_GO=1;
}

int TOMArEngine::RealyNewGame(){
	int i,j,g=0;
	if(GAME_IS_GO) GAME_IS_GO=0,g=1;
	GrPtr=GrArea;
	for(i=64; i<=240; i++)
		for(j=32; j<=159; j++)
			PIXEL(i,j)=((i&j)&63);
	bar(80,48,223,144,146);
	RED_CHARS;
	String("‚ˆŒ€ˆ…!",129,60);
	String("‚ë ã¦¥ § ¯ãáâ¨«¨ ¨£àã.",90,88);
	String("…á«¨ ¢ë ®â¢¥â¨â¥ 'Y',",93,97);
	String("¢®ááâ ­®¢¨âì ¥ñ ¡ã¤¥â",93,106);
	String("­¥¢®§¬®¦­®!",123,115);
	GREEN_CHARS;
	while(1){
		if(key[iN]){ GrPtr=GrBuff; SWBUFF(); if(g) GAME_IS_GO=1; return 0; }
		else if(key[iY]){ GrPtr=GrBuff; SWBUFF(); if(g) GAME_IS_GO=1; return 1; }
	}
}

void TOMArEngine::CreateOptionsFile(){
	FILE *f=fopen("options.txt","w+b"); if(!f) return;
	for(int i=0; SAVE_FILE[i]!='.' && SAVE_FILE[i]!=0; i++);
	SAVE_FILE[i++]=0;
	fprintf(f,
		"\tOMAr's options\r\n\tÄÄÄÄÄÄÄÄÄÄÄÄÄÄ\r\n"
		"MAN = %i\r\nITEMS = %i\r\nSAVE = %s\r\n"
		"LEVELS = %i\r\nTIME LIMIT = %i\r\nGAMMA = %i\r\n"
		"0 PLAYER = %s,%i,%i,%i\r\n1 PLAYER = %s,%i,%i,%i\r\n"
		"2 PLAYER = %s,%i,%i,%i\r\n",
		Options.n_man,Options.items,SAVE_FILE,Options.levels_to_go,
		(1+Options.time_limit/1092),Options.gamma,
		NAME[0],MAN_PRICE[0],MAN_HLTH[0],IX_SCR[0],
		NAME[1],MAN_PRICE[1],MAN_HLTH[1],IX_SCR[1],
		NAME[2],MAN_PRICE[2],MAN_HLTH[2],IX_SCR[2]
	);
	fprintf(f,"WEAPONS = ");
	for(i=1; i<NUM_WEAP; i++) fprintf(f,"%i",LIC_WEA[i]);
	fprintf(f,"\r\nCONTROL = ");
	for(i=0; i<3; i++) fprintf(f,"%i",CONTROL[i]);
	fprintf(f,"\r\n"); fclose(f);
	AddSavExt(SAVE_FILE);
}

void TOMArEngine::LoadOptions(){
	FILE *f=fopen("options.txt","rt");
	Options.Def();
	if(f==NULL){ CreateOptionsFile(); return; }
	char s[128],s2[255]; int minutes,i,j,n;
	while(fgets(s,127,f)){
		if(s[0]=='M' && s[1]=='A' && s[2]=='N') Options.n_man=GetNumber(s);
//		else if(s[0]=='G' && s[1]=='O' && s[2]=='L') Options.gold=GetNumber(s);
		else if(s[0]=='I' && s[1]=='T' && s[2]=='E') Options.items=GetNumber(s);
		else if(s[0]=='T' && s[1]=='I' && s[2]=='M') minutes=GetNumber(s);
		else if(s[0]=='L' && s[1]=='E' && s[2]=='V') Options.levels_to_go=GetNumber(s);
		else if(s[0]=='G' && s[1]=='A' && s[2]=='M') Options.gamma=GetNumber(s);
		else if(s[0]=='S' && s[1]=='A' && s[2]=='V'){
			for(i=2; i<127 && s[i]!='='; i++);
			while(s[++i]==' ');
			if(i>120) Error(STR_DTA[0]);
			int j=0;
			while(s[i] && s[i]!='\n' && s[i]!='\r' && j<8)
				SAVE_FILE[j++]=s[i++];
			SAVE_FILE[j]=0;
			AddSavExt(SAVE_FILE);
		}else if(s[0]=='W' && s[1]=='E' && s[2]=='A'){
			for(i=2; i<127 && s[i]!='='; i++);
			while(s[++i]==' ');
			if(i>120) Error(STR_DTA[0]);
			int j=1; char num=0; s2[1]=0;
			while(s[i]!='\r' && s[i]!='\n' && j<NUM_WEAP){
				s2[0]=s[i++];
				num=atoi(s2);
				if(num<5) LIC_WEA[j++]=num;
			}
		}else if(s[0]=='C' && s[1]=='O' && s[2]=='N'){
			for(i=2; i<127 && s[i]!='='; i++);
			while(s[++i]==' ');
			if(i>120) Error(STR_DTA[0]);
			int j=0,num=0,ctl[3]={0,1,2}; s2[1]=0;
			while(s[i]!='\r' && s[i]!='\n' && j<3){
				s2[0]=s[i++];
				num=atoi(s2);
				if(num<3) ctl[j++]=num;
			}
			if(ctl[0]+ctl[1]+ctl[2]==3)
				for(i=0; i<3; i++) CONTROL[i]=ctl[i];
		}else if(isinum(s[0])){
			s2[0]=s[0]; s2[1]=0; int nM=atoi(s2);
			if(nM<0 || nM>2) Error(STR_DTA[0]);
			for(i=2; i<127 && s[i]!='='; i++);
			while(s[++i]==' ');
			if(i>120) Error(STR_DTA[0]);
			int j=0;
			while(s[i]!='\n' && s[i]!=',' && j<15) NAME[nM][j++]=s[i++];
			NAME[nM][j]=0;
			if(s[i]==','){
				while(!isfnum(s[++i]));
				for(j=0; (isfnum(s[i]) && j<49); j++,i++)
					s2[j]=s[i];
				s2[j]=0; n=atoi(s2);
				if(n<100) n=100; else if(n>500) n=500;
				MAN_PRICE[nM]=n;
			}
			while(s[i]!='\n' && s[i]!=',' && i<49) i++;
			if(s[i]==','){
				while(!isfnum(s[++i]));
				for(j=0; (isfnum(s[i]) && j<49); j++,i++)
					s2[j]=s[i];
				s2[j]=0; n=atoi(s2);
				if(n<129 && n>=7) MAN_HLTH[nM]=n;
			}
			while(s[i]!='\n' && s[i]!=',' && i<49) i++;
			if(s[i]==','){
				while(!isfnum(s[++i]));
				for(j=0; (isfnum(s[i]) && j<49); j++,i++)
					s2[j]=s[i];
				s2[j]=0; n=atoi(s2);
				if(n>=0 && n<3)
					IX_SCR[nM]=n;
			}
		}
	}
	LIC_WEA[0]=4;
	Options.Check();
	if(minutes>60 || minutes<0) minutes=10;
	Options.time_limit=((float)minutes*18.2*60.0);
	fclose(f);
}

#if !IS_DEBUG
int TOMArEngine::CheckEXE(){
	DWORD check_sum=0L,num,flen=0L;
	int hf=_open("omar.exe",O_RDONLY); if(hf==-1) goto L1;
	lseek(hf,0,0);
	while(_read(hf,&num,4)==4){
		check_sum+=num;
		asm jnc L0;
		check_sum++;
L0:
	}
	if(check_sum!=FTR.exe_checksum) goto L1;
	lseek(hf,0,2); flen=tell(hf); _close(hf);
	if(flen!=*exe_len){
L1:
		Error(STR_DTA[4]);
	}
	return 1;
}
#endif