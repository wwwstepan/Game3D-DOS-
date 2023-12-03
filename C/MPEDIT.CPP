#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <dos.h>
#include <mem.h>
#include "c\obj.h"
#include "c\engine.h"

// void TLevel::CheckTelep(){
// void TLevel::Clear(){
// int TLevel::SetTelep(int x,int y,int t_x,int t_y){
// void TLevel::ClearTelep(int x,int y){
// TLevel *LevCpy1,*LevCpy2;
// TEditor::TEditor(){
// void TEditor::Tex32(int x,int y,int n){
// int TEditor::RdMap(){
// int TEditor::WrMap(){
// void TEditor::Bar(int i,int j){
// void TEditor::DrawMap(){
// void TEditor::ShowTPort(){
// void TEditor::Run(){
// void TEditor::WaitMouse(){
// int TEditor::Bottons(){
// void TEditor::Edit(){
// void TOMArEngine::MapEditor(){

const int
	SIZE_ICON=200/WORLD_ROW,
	MAX_X=(WORLD_COL*SIZE_ICON),
	MAX_Y=(WORLD_ROW*SIZE_ICON);

struct TLevel{
	char map[WORLD_COL][WORLD_ROW];
	char tel_x[128],tel_y[128];
	void Clear();
	int SetTelep(int x,int y,int t_x,int t_y);
	void ClearTelep(int x,int y);
   void CheckTelep();
};

void TLevel::CheckTelep(){
	int i,j;
	char is_tel[128];
	setmem(is_tel,128,0);
	for(i=0; i<WORLD_COL; i++)
		for(j=0; j<WORLD_ROW; j++)
			if(map[i][j]>=128)
				is_tel[map[i][j]-128]=1;
	for(i=0; i<128; i++)
		if(!is_tel[i])
			tel_x[i]=tel_y[i]=0;
}

void TLevel::Clear(){
	for(int i=0; i<WORLD_COL; i++)
		for(int j=0; j<WORLD_ROW; j++)
			map[i][j]=(i>=4 && j>=4 && i<WORLD_COL-4 && j<WORLD_ROW-4)?0:2;
	setmem(tel_x,256,0);
}

int TLevel::SetTelep(int x,int y,int t_x,int t_y){
	if(map[x][y]>=128){
		char ti=map[x][y]-128;
		tel_x[ti]=t_x;
		tel_y[ti]=t_y;
		return 1;
	}
	if(t_x>=0 && t_y>=0 && t_x<WORLD_COL && t_y<WORLD_ROW)
		for(int i=0; i<128; i++)
			if(tel_x[i]==0 && tel_y[i]==0){
				tel_x[i]=t_x; tel_y[i]=t_y;
				map[x][y]=i+128;
				return 1;
			}
	return 0;
}

void TLevel::ClearTelep(int x,int y){
	char ti=map[x][y];
	if(ti>=128){
		map[x][y]=0;
		for(int i=0; i<WORLD_COL; i++)
			for(int j=0; j<WORLD_ROW; j++)
				if(map[i][j]==ti) return;
		ti-=128;
		tel_x[ti]=tel_y[ti]=0;
	}
}
TLevel *LevCpy1,*LevCpy2;

class TEditor{
	int hfile;
	int numMap,numWall;
	TLevel lev;
	void Edit();
	void Bar(int i,int j);
	void DrawMap();
	void ShowTPort();
	void Tex32(int x,int y,int n);
	void botton(int x,int y,int n);
	int RdMap();
	int WrMap();
	void WaitMouse();
	int Bottons();
public:
	void Run();
	TEditor();
	~TEditor(){}
};

TEditor::TEditor(){
	numMap=0;
	numWall=2;
}

void TEditor::Tex32(int x,int y,int n){
	if(n!=1){
		char *p=pWall+((n-1)<<10);
		char *P=GrPtr+(y<<8)+(y<<6)+x;
		for(int i=0; i<32; i++,P+=(320-32))
			for(int j=0; j<32; j++,P++)
				*P=p[(j<<5)+i];
	}else{
		bar(x,y,x+32,y+32,3);
		String("Tele",x+3,y+10);
		String("port",x+3,y+16);
	}
}

int TEditor::RdMap(){
	int hf=_open(RES_FILE_NAME,O_RDONLY);
	if(hf==-1) return 0;
	if(lseek(hf,FTR.ofs_levels+(((long)numMap)*((long)LEVEL_DISK_SIZE)),0)!=-1L){
		if(_read(hf,&lev,LEVEL_DISK_SIZE)<LEVEL_DISK_SIZE) lev.Clear();
	}
	_close(hf);
	lev.CheckTelep();
	return 1;
}

int TEditor::WrMap(){
	int hf=_open(RES_FILE_NAME,O_RDWR);
	if(hf==-1) return 0;
	if(lseek(hf,FTR.ofs_levels+(((long)numMap)*((long)LEVEL_DISK_SIZE)),0)!=-1L)
		_write(hf,&lev,LEVEL_DISK_SIZE);
	_close(hf);
	return 1;
}

void TEditor::Bar(int i,int j){
	int x=i*SIZE_ICON, y=j*SIZE_ICON;
	if(!lev.map[i][j]){
		bar(x,y,x+SIZE_ICON,y+SIZE_ICON,0);
	}else if(lev.map[i][j]>=128){
		bar(x,y,x+SIZE_ICON,y+SIZE_ICON,175);
	}else if(lev.map[i][j]>0 && lev.map[i][j]<=MAX_WALL){
		char *p=pWall+((lev.map[i][j]-1)<<10);
		for(int J=y; J<y+SIZE_ICON; J++)
			for(int I=x; I<x+SIZE_ICON; I++)
				PIXEL(I,J)=*(p+=111);
	}
}

void TEditor::DrawMap(){
	bar(0,0,MAX_X,MAX_Y,0);
	for(int i=0; i<WORLD_COL; i++)
		for(int j=0; j<WORLD_ROW; j++)
			Bar(i,j);
}

void TEditor::ShowTPort(){
	for(int i=0; i<WORLD_COL; i++)
		for(int j=0; j<WORLD_ROW; j++){
			UNS ti=lev.map[i][j];
			if(ti>=128){
				ti-=128;
				line(i*SIZE_ICON+1,j*SIZE_ICON+1,
					lev.tel_x[ti]*SIZE_ICON+1,lev.tel_y[ti]*SIZE_ICON+1,13);
			}
		}
}

void TEditor::Run(){
	if(LevCpy1) memcpy(LevCpy1,&lev,LEVEL_DISK_SIZE);
	if(LevCpy2) memcpy(LevCpy2,&lev,LEVEL_DISK_SIZE);
   GrPtr=GrArea;
	Clear64K(GrArea);
	String("EXIT",280,1);
	String("SAVE",280,11);
	String("MAP\x7",280,21);
	String("MAP\xB",280,31);
	String("WALL\x7",280,41);
	String("WALL\xB",280,51);
	String("CLEAR",280,61);
	Edit();
	GrPtr=GrBuff;
	SWBUFF();
}

void TEditor::WaitMouse(){
	MouseHide();
	while(MOUSE.b){
		MOUSE.Get();
		MouseShow();
		delay(1);
		MouseHide();
	}
}

int TEditor::Bottons(){
	if(MOUSE.x>280 && MOUSE.y<70){
		if(MOUSE.y<10){
			WaitMouse();
			if(MOUSE.x>280 && MOUSE.y<10) return 1;
		}else if(MOUSE.y<20){
			WaitMouse();
			if(MOUSE.x>280 && MOUSE.y>10 && MOUSE.y<20); WrMap();
		}else if(MOUSE.y<30){
			WaitMouse();
			if(MOUSE.x>280 && MOUSE.y>20 && MOUSE.y<30){
				if(!WrMap()) return 1;
				if(++numMap>49) numMap=0;
				if(!RdMap()) return 1;
				DrawMap();
			}
		}else if(MOUSE.y<40){
			WaitMouse();
			if(MOUSE.x>280 && MOUSE.y>30 && MOUSE.y<40){
				if(!WrMap()) return 1;
				if(--numMap<0) numMap=49;
				if(!RdMap()) return 1;
				DrawMap();
			}
		}else if(MOUSE.y<50){
			WaitMouse();
			if(MOUSE.x>280 && MOUSE.y>40 && MOUSE.y<50){
				if((++numWall)>MAX_WALL) numWall=1;
				Tex32(286,100,numWall);
			}
		}else if(MOUSE.y<60){
			WaitMouse();
			if(MOUSE.x>280 && MOUSE.y>50 && MOUSE.y<60){
				if((--numWall)<1) numWall=MAX_WALL;
				Tex32(286,100,numWall);
			}
		}else{
			WaitMouse();
			if(MOUSE.x>280 && MOUSE.y>60 && MOUSE.y<70){
				MouseHide();
				bar(50,75,150,125,6); bar(55,78,145,122,2);
				bar(60,100,95,120,20); bar(105,100,140,120,20);
				String("Очистить карту?",55,86);
				String("YES",66,106); String("NO",115,106);
				while(!MOUSE.b && !key[iY] && !key[iN] && !key[iESC]){
					MOUSE.Get(); MouseShow(); delay(1); MouseHide();
				}
				if(key[iY] || (MOUSE.x>=60 && MOUSE.x<=95 && MOUSE.y>=100 && MOUSE.y<=120))
					lev.Clear();
				DrawMap();
				WaitMouse(); while(key[iESC]);
			}
		}
	}
	return 0;
}

void TEditor::Edit(){
	if(!RdMap()) return;
	DrawMap();
	int i,j,t_x,t_y;
	Tex32(286,100,numWall);
	while(!key[iESC] && !key[iF10]){
		MOUSE.Get();
		i=MOUSE.x/SIZE_ICON; j=MOUSE.y/SIZE_ICON;
		MouseShow();
		if(MOUSE.b){
			if(MOUSE.x<MAX_X && MOUSE.y<MAX_Y){
				if(MOUSE.b==1){
					if(numWall==1){
						ShowTPort();
						MouseHide();
						while(MOUSE.b) MOUSE.Get();
						while(!MOUSE.b){
							MOUSE.Get();
							xline(i*SIZE_ICON+2,j*SIZE_ICON+3,MOUSE.x,MOUSE.y,15);
							delay(10);
							xline(i*SIZE_ICON+2,j*SIZE_ICON+3,MOUSE.x,MOUSE.y,15);
						}
						t_x=MOUSE.x/SIZE_ICON; t_y=MOUSE.y/SIZE_ICON;
						if(t_x>=0 && t_x<WORLD_COL && t_y>=0 && t_y<WORLD_ROW){
							lev.SetTelep(i,j,t_x,t_y);
						}
						while(MOUSE.b) MOUSE.Get();
						MouseShow();
						DrawMap();
					}else{
						if(numWall>WALL_SAND){
							if(i>0 && i<WORLD_COL-1 && j>0 && j<WORLD_ROW-1)
								lev.map[i][j]=numWall;
						}else{
							if(i>=0 && i<WORLD_COL && j>=0 && j<WORLD_ROW)
								lev.map[i][j]=numWall;
						}
					}
				}else if(i>0 && i<WORLD_COL-1 && j>0 && j<WORLD_ROW-1){
					if(lev.map[i][j]<128) lev.map[i][j]=0;
					else lev.ClearTelep(i,j);
				}
			}else{ if(Bottons()) return; }
		}
		if(key[iSPACE]){
			ShowTPort();
			while(key[iSPACE]) MouseHide(),MOUSE.Get(),MouseShow(),delay(5);
			DrawMap();
		}
		if(key[iF5] && LevCpy1) memcpy(LevCpy1,&lev,LEVEL_DISK_SIZE);
		if(key[iF6] && LevCpy2) memcpy(LevCpy2,&lev,LEVEL_DISK_SIZE);
		if(key[iF7] && LevCpy1) memcpy(&lev,LevCpy1,LEVEL_DISK_SIZE),DrawMap();
		if(key[iF8] && LevCpy2) memcpy(&lev,LevCpy2,LEVEL_DISK_SIZE),DrawMap();
		delay(10); MouseHide();
		if(i>=0 && i<WORLD_COL && j>=0 && j<WORLD_ROW){
			Bar(i,j);
			if(lev.map[i][j]<=MAX_WALL && lev.map[i][j]>0)
				Tex32(286,140,lev.map[i][j]);
			else bar(286,140,318,176,0);
		}else bar(286,140,318,176,0);
	}
}

void TOMArEngine::MapEditor(){
	while(key[iESC] || key[iENTER]);
	TEditor *ed=new TEditor;
	if(ed){
		LevCpy1=new TLevel;
		LevCpy2=new TLevel;
		ed->Run();
		if(LevCpy1) delete LevCpy1;
		if(LevCpy2) delete LevCpy2;
		delete ed;
	}
}