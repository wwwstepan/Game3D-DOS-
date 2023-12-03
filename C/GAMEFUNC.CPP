#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "c\3d.h"
#include "c\obj.h"
#include "c\engine.h"

// void TOMArEngine::AllocMem(){
// void TOMArEngine::FreeMem(){
// void TOMArEngine::____CA(){
// void TOMArEngine::swLine(UNS x1, UNS y1, UNS x2, UNS y2){
// void TOMArEngine::SwBuffOrn(){
// void TOMArEngine::GrowDark(int type,int dl){
// void TOMArEngine::SetPal(int n){
// void TOMArEngine::MakeTables(){
// int TOMArEngine::LoadRes(){
// void TOMArEngine::Quit(){ FreeMem(); mode(3); exit(1); }
// void TOMArEngine::AddSavExt(char *s){
// void TOMArEngine::ShowWhyIsEndLevel(){

void TOMArEngine::AllocMem(){
	GrBuff=new char[64000U];
	if(GrBuff==NULL) Error(STR_DTA[6]);
	GrPtr=GrBuff;
	pWall=new char[28672]; pBurst=new char[9216]; pFon6=new char[9216];
	pFacet=new char[23040]; _n_=new char[32];
	SIN_TABLE=new int[ANGLE_360+ANGLE_90+1];
	tan_table=new float[ANGLE_360+1]; inv_tan_table=new float[ANGLE_360+1];
	y_step=new float[ANGLE_360+1]; x_step=new float[ANGLE_360+1];
	cos_table=new float[ANGLE_360+1]; inv_cos_table=new float[ANGLE_360+1];
	inv_sin_table=new float[ANGLE_360+1]; LightLev=new char[8220];
	if(FP_OFF(LightLev)>15) Error(STR_DTA[7]);
	light_seg=(++FP_SEG(LightLev)); FP_OFF(LightLev)=0;
	if( LightLev==NULL || pWall==NULL || pFon6==NULL || pFacet==NULL ||
		pBurst==NULL || _n_==NULL || tan_table==NULL || inv_tan_table==NULL ||
		y_step==NULL || x_step==NULL || cos_table==NULL ||
		inv_cos_table==NULL || inv_sin_table==NULL || SIN_TABLE==NULL)
			Error(STR_DTA[6]);
}

void TOMArEngine::FreeMem(){
	delete[] LightLev,GrBuff,pWall,pBurst,pFon6,pFacet,_n_,SIN_TABLE,tan_table,
		inv_tan_table,y_step,x_step,cos_table,inv_cos_table,inv_sin_table;
}

void TOMArEngine::____CA(){
	char *p=pWall,ptr[1024]; int i;
	for(i=0; i<1024; i++) ptr[i]=p[i];
	for(i=1; i<1024; i++)
		if(p[i]<255){
			int s=p[i-1]+p[i+1]+p[i-32]+p[i+32]+p[i-31]+p[i+31]+p[i-33]+p[i+33];
			s>>=3; s++; if(s>=255) s=224; ptr[i]=s;
		}
	for(i=0; i<1024; i++) p[i]=ptr[i];
}

void TOMArEngine::swLine(UNS x1, UNS y1, UNS x2, UNS y2){
	int t, dist, xerr=0, yerr=0, incx, incy;
	int dx=x2-x1, dy=y2-y1, s;
	incx=dx>0?1:dx==0?0:-1; incy=dy>0?1:dy==0?0:-1;
	dx=dx>0?dx:-dx; dy=dy>0?dy:-dy;
	if(dx>dy) dist=dx; else dist=dy;
	for(t=0; t<=dist+1; t++){
		if(x1<320 && y1<200){
			s=(y1<<8)+(y1<<6)+x1;
			GrArea[s]=GrBuff[s];
		}
		xerr+=dx; yerr+=dy;
		if(xerr>dist){ xerr-=dist; x1+=incx; }
		if(yerr>dist){ yerr-=dist; y1+=incy; }
	}
}

void TOMArEngine::SwBuffOrn(){
	int x[4]={0,319,80,240},y[4]={60,140,0,199},i;
	for(i=0; i<160; i++){
		swLine(x[0]+30,y[0]-55,x[0]-30,y[0]+50);
		swLine(x[1]-30,y[1]-50,x[1]+30,y[1]+55);
		swLine(x[2],y[2],x[2]+75,y[2]-30);
		swLine(x[2],y[2],x[2]-75,y[2]-30);
		swLine(x[3],y[3],x[3]+75,y[3]+30);
		swLine(x[3],y[3],x[3]-75,y[3]+30);
		delay(7); x[0]+=2; x[1]-=2; y[2]+=2; y[3]-=2;
	}
	for(i=0; i<200; i++) swLine(0,i,319,i),delay(3);
}

void TOMArEngine::GrowDark(int type,int dl){
	UNS i,j;
	switch(type){
	case 0:
		for(i=0; i<64000U; i++) GrArea[i]&=31;
		for(i=0; i<300; i++){
			for(j=0; j<63; j++){
				if(RGB_COLOR[j].red>0)
					RGB_COLOR[j].red=(float)(RGB_COLOR[j].red)/1.031;
				if(RGB_COLOR[j].green>0)
					RGB_COLOR[j].green=(float)(RGB_COLOR[j].green)/1.031;
				if(RGB_COLOR[j].blue>0)
					RGB_COLOR[j].blue=(float)(RGB_COLOR[j].blue)/1.031;
				SetPal(i);
			} delay(dl);
		}
		Clear64K(GrArea);
		break;
	case 1:
		for(i=0; i<250; i++){
			for(j=0; j<256; j++){
				if(RGB_COLOR[j].red>0)
					RGB_COLOR[j].red=(float)(RGB_COLOR[j].red)/1.031;
				if(RGB_COLOR[j].green>0)
					RGB_COLOR[j].green=(float)(RGB_COLOR[j].green)/1.031;
				if(RGB_COLOR[j].blue>0)
					RGB_COLOR[j].blue=(float)(RGB_COLOR[j].blue)/1.031;
				SetPal(i);
			} delay(dl);
		}
		Clear64K(GrArea);
		break;
	case 2:
		GrPtr=GrArea;
		for(long l=500000L; l--;){
			int x,y; x=random(320); y=random(200); PIXEL(x,y)=0;
		}
		GrPtr=GrBuff;
		Clear64K(GrArea);
		break;
	case 3:
		for(l=500000L; l--;){
			int x,y; x=random(320); y=random(200);
			UNS offs=x+(y<<6)+(y<<8);
			GrArea[offs]=GrBuff[offs];
		}
		for(i=0; i<64000U; i++) GrArea[i]=GrBuff[i];
	}
}

void TOMArEngine::SetPal(int n){
	outp(PAL_MASK,0xFF);
	outp(PAL_WR,n);
	outp(PAL_RGB,RGB_COLOR[n].red);
	outp(PAL_RGB,RGB_COLOR[n].green);
	outp(PAL_RGB,RGB_COLOR[n].blue);
}

void TOMArEngine::MakeTables(){
	int ang; float rad_ang;
	int i,j;
	for(i=0; i<128; i++)
		for(j=0; j<128; j++){
			tab_angle[i][j]=.5+(_Angle(i-64,j-64)*(float)ANGLE_360/6.283);
			if(tab_angle[i][j]>ANGLE_180) tab_angle[i][j]-=ANGLE_360;
			else if(tab_angle[i][j]<=-ANGLE_180) tab_angle[i][j]+=ANGLE_360;
		}
	for(i=0; i<64; i++)
		for(j=0; j<64; j++)
			tab_distance[i][j]=(hypot(i,j)+.5);
	for(ang=0; ang<ANGLE_360+ANGLE_90; ang++)
		SIN_TABLE[ang]=sin(((float)ang*3.14159)/(float)ANGLE_180)*512.0;
	for(ang=ANGLE_0; ang<=ANGLE_360; ang++){
		rad_ang=(3.272e-4)+ang*2*3.141592654/ANGLE_360;
		tan_table[ang]=tan(rad_ang);
		inv_tan_table[ang]=1/tan_table[ang];
		if(ang>=ANGLE_0 && ang<ANGLE_180)
			y_step[ang]=fabs(tan_table[ang]*CELL_SIZE);
		else y_step[ang]=-fabs(tan_table[ang]*CELL_SIZE);
		if(ang>=ANGLE_90 && ang<ANGLE_270)
			x_step[ang]=-fabs(inv_tan_table[ang]*CELL_SIZE);
		else x_step[ang]=fabs(inv_tan_table[ang]*CELL_SIZE);
		inv_cos_table[ang]=1/cos(rad_ang);
		inv_sin_table[ang]=1/sin(rad_ang);
	}
	for(ang=-ANGLE_30; ang<=ANGLE_30; ang++){
		rad_ang=(3.272e-4)+ang*2*3.141592654/ANGLE_360;
		cos_table[ang+ANGLE_30]=1/cos(rad_ang);
	}
}

int TOMArEngine::LoadRes(){
	int i,j;
	int hf=_open(RES_FILE_NAME,O_RDONLY);
	long szofFooter=sizeof(FTR);
	lseek(hf,-szofFooter,2);
	_read(hf,&FTR,szofFooter);
	if(FTR.Check()) Error(STR_DTA[5]);
	lseek(hf,FTR.ofs_lights,0);
	_read(hf,LightLev,8192);
	_read(hf,pWall+1024U,FTR.sz_wall);
	_read(hf,pBurst,FTR.sz_burst);
	_read(hf,pFacet,FTR.sz_facet);
	_read(hf,pFon6,FTR.sz_font);
	GREEN_CHARS;
	for(i=0; i<32; i++)
		for(j=0; j<32; j++)
			pWall[(j<<5)+i]=((i==0) || (j==0) || (i==31) || (j==31))?255:224;
	char n_v,n_f;
	lseek(hf,FTR.ofs_models,0);
	for(i=0; i<FTR.n_models && i<MAX_SHAPES; i++){
		_read(hf,&n_v,1); _read(hf,&n_f,1);
		shape[i].num_vec=n_v; shape[i].num_fac=n_f;
		shape[i].V=new TVector[n_v];
		shape[i].F=new TFacet[n_f];
		_read(hf,&(shape[i].V[0].x),6*n_v);
		_read(hf,&(shape[i].F[0].i[0]),4*n_f);
	}
	_close(hf); return 1;
}

void TOMArEngine::Quit(){ FreeMem(); mode(3); exit(1); }

void TOMArEngine::AddSavExt(char *s){
	for(int i=0; i<8 && s[i]; i++);
	s[i++]='.'; s[i++]='s'; s[i++]='a'; s[i++]='v'; s[i]=0;
}

void TOMArEngine::ShowWhyIsEndLevel(){
	UNS i;
	Clear64K(GrBuff);
	int delay_time=1111,x;
	for(i=0; i<3; i++){
		x=i*105+55;
		if(GAME_OVER==1){
			StringCenter("Собрали",x,85);
			StringCenter("все",x,95);
			StringCenter("золото",x,105);
		}else if(GAME_OVER==2){
			StringCenter("Время истекло",x,96);
		}else if(GAME_OVER==3){
			StringCenter("Убили",x,80);
			StringCenter("всех",x,92);
			StringCenter("МОНСТРОВ",x,104);
		}else{
			UNS n=GAME_OVER-10;
			if(n<=NUM_OPPONENT)
				StringCenter(NAME[Man[n]->number],x,80);
			StringCenter("обессилел.",x,90);
			StringCenter("ПУСТЬ ОН",x,100);
			StringCenter("ОТДОХНЕТ",x,110);
			delay_time=1777;
		}
	}
	GrowDark(3,5);
	delay(delay_time);
}
