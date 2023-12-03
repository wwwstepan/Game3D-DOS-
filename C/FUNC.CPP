#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>
#include "c\defs.h"
#include "c\debug.h"

// void interrupt far Key_Int(...){
// void interrupt far Time_Int(...){
// TKeyboard::TKeyboard(){
// TKeyboard::~TKeyboard(){
// int Angle(int x,int y){
// int Distance(int x,int y){
// float _Angle(float x,float y){
// void swap(char &a,char &b){ register char i=a; a=b; b=i; }
// void swap(char &a,char &b,char &c){ register char i=a; a=b; b=c; c=i; }
// void StringRight(char far *str,int x,int y){
// void StringCenter(char far *str,int x,int y){
// void String(long number,int x,int y){
// void ClrCharAt(char c,int x,int y){
// void String(char far *str,int x,int y){
// void Error(const char far* message){
// int isinum(char c){ return ((c>='0' && c<='9') || (c=='-') || (c=='+')); }
// int isfnum(char c){ return ((isinum(c)) || (c=='.')); }
// void Tri(int x1,int y1,int x2,int y2,int x3,int y3){
// void TriFullScr(int x1,int y1,int x2,int y2,int x3,int y3){
// char key_pressed(){
// void i2a(unsigned long num,char *s,int len){
// void move64K(char *v,char *iz){
// void Clear64K(char far *_Ptr_){

void interrupt far (*Old_Key)(...);
void interrupt far (*Old_Time)(...);

void interrupt far Key_Int(...){
asm{
	sti
	in al,60h  // get the key that was pressed
	xor ah,ah
	mov bx,ax
	mov raw_key,ax
	in al,61h  // set the control register
	or al,82h  // set the proper bits to reset the FF
	out 61h,al // send the new data back to the control register
	and al,7Fh
	out 61h,al // complete the reset
	mov al,20h
	out 20h,al // re-enable interrupts
	cmp bl,127
	mov si,OFFSET key
	ja short L0
	mov BYTE PTR[si+bx],1
	jmp short L1
}L0:
	asm sub bx,128
	asm mov BYTE PTR[si+bx],0
L1:
	if(key[iCTRL] && ((key[iALT] && key[iDEL]) || key[iF10])){ mode(3); exit(1); }
}

void interrupt far Time_Int(...){
	Old_Time(); sw_buff=1;
	tic++;
	if(GAME_IS_GO && TIME_LEFT) TIME_LEFT--;
}

TKeyboard::TKeyboard(){
	Old_Key=_dos_getvect(KEYBOARD_INT);
	_dos_setvect(KEYBOARD_INT,Key_Int);
	Old_Time=_dos_getvect(TIMER_INT);
	_dos_setvect(TIMER_INT,Time_Int);
}

TKeyboard::~TKeyboard(){
	_dos_setvect(KEYBOARD_INT,Old_Key);
	_dos_setvect(TIMER_INT,Old_Time);
}

int Angle(int x,int y){
	if(abs(x)>64 || abs(y)>64){
		x>>=3; y>>=3;
		if(abs(x)>64 || abs(y)>64){
			x>>=3; y>>=3;
			if(abs(x)>64 || abs(y)>64){
				x>>=3; y>>=3;
				if(abs(x)>64 || abs(y)>64)
					return 0;
	}	}	}
	return tab_angle[x+64][y+64];
}

int Distance(int x,int y){
	x=abs(x); y=abs(y);
	if(x<64 && y<64) return tab_distance[x][y];
	x>>=3; y>>=3;
	if(x<64 && y<64) return tab_distance[x][y]<<3;
	x>>=3; y>>=3;
	if(x<64 && y<64) return tab_distance[x][y]<<6;
	x>>=3; y>>=3;
	if(x<64 && y<64) return tab_distance[x][y]<<9;
	return 32767;
}

float _Angle(float x,float y){
	float r=hypot(x,y); if(r<2e-8) return 0;
	float a=acos(x/r);
	if(y<0) a=6.28-a;
//	if(a>3.14) a-=6.28; else if(a<-3.14) a+=6.28;
	return a;
}

void swap(char &a,char &b){ register char i=a; a=b; b=i; }
void swap(char &a,char &b,char &c){ register char i=a; a=b; b=c; c=i; }

void StringRight(char far *str,int x,int y){
	for(int l=0; str[l] && str[l]!='\xFF'; l++);
	String(str,x-(l<<2)-(l<<1),y);
}

void StringCenter(char far *str,int x,int y){
	for(int l=0; str[l] && str[l]!='\xFF'; l++);
	String(str,x-l-(l<<1),y);
}

void String(long number,int x,int y){
	ltoa(number,_n_,10);
	char *str=_n_;
	if(y<0 || y>193) return;
	while(*str && x<312){
		tex_n=*str; tex_x=x; tex_top=y; PUTCH6(); x+=6; str++;
	}
}

void ClrCharAt(char c,int x,int y){
	tex_n=c; tex_x=x; tex_top=y; PUTCH6COLORED();
}

void String(char far *str,int x,int y){
	int X=x-6;
	if(y<0 || y>193) return;
	while(*str && x<312){
		if(*str=='\xFF') x=X,y+=6;
		else if(*str!=' '){ tex_n=*str; tex_x=x; tex_top=y; PUTCH6(); }
		x+=6; str++;
	}
}

void Error(const char far* message){
	mode(3); printf("\n%s\n",message);
	raw_key=0; while(!raw_key); exit(1);
}

int isinum(char c){ return ((c>='0' && c<='9') || (c=='-') || (c=='+')); }
int isfnum(char c){ return ((isinum(c)) || (c=='.')); }

unsigned long tex_index3,tex_dy2;

void Tri(int x1,int y1,int x2,int y2,int x3,int y3){
	int dx1=x2-x1,dy1=y2-y1,dx2=x3-x2,dy2=y3-y2,dx3=x1-x3,dy3=y1-y3;
	if(dx1==0) dx1=1,x2++; if(dx2==0) dx2=1,x3++; if(dx3==0) dx3=-1,x1--;
	if( (dx1*dy2-dx2*dy1)<0 ){
		int t=dx1; dx1=-dx3; dx3=-t; t=dy1; dy1=-dy3; dy3=-t;
		t=x2; x2=x3; x3=t; t=y2; y2=y3; y3=t; dx2=-dx2; dy2=-dy2;
	}
	int dx4,dy4,d1=1,d2=1,d3=1,d4=1,t=0;
	if(dy1>0) t++; if(dy2>0) t++; if(dy3>0) t++;
	tex_dy=1048576L;
	if(t==2){
		if(dy1<0) tex_dy/=-dy1;
		else if(dy2<0) tex_dy/=-dy2;
		else if(dy3<0) tex_dy/=-dy3;
	}else{
		if(dy1>0) tex_dy/=dy1;
		else if(dy2>0) tex_dy/=dy2;
		else if(dy3>0) tex_dy/=dy3;
	}
	t=0; if(dx1>=0) t++; if(dx2>=0) t++; if(dx3>=0) t++;
	if(t==2){
		if(dx1<0){
			t=x1; x1=x2; x2=x3; x3=t; y1=y2;
			if(dy2<0) d1=-1; if(dy3<0) d2=-1; if(dy1>=0) d3=d4=-1;
			dx4=dx1; dy4=dy1; dx1=dx2; dy1=dy2;
			dx2=dx3; dy2=dy3; dx3=dx4; dy3=dy4;
		}else if(dx2<0){
			t=x1; x1=x3; x3=x2; x2=t; y1=y2=y3;
			if(dy3<0) d1=-1; if(dy1<0) d2=-1; if(dy2>=0) d3=d4=-1;
			dx4=dx2; dy4=dy2; dx2=dx1; dy2=dy1;
			dx1=dx3; dy1=dy3; dx3=dx4; dy3=dy4;
		}else if(dx3<0){
			y2=y1;
			if(dy1<0) d1=-1; if(dy2<0) d2=-1; if(dy3>=0) d3=d4=-1;
			dx4=dx3; dy4=dy3;
		}
	}else{
		if(dx1>=0){
			t=x2; x2=x3; x3=t; y2=y1;
			if(dy1<0) d1=d2=-1; if(dy3>=0) d3=-1; if(dy2>=0) d4=-1;
			dx4=dx2; dy4=dy2; dx2=dx1; dy2=dy1;
		}else if(dx2>=0){
			t=x1; x1=x2; x2=t; y1=y2;
			if(dy2<0) d1=d2=-1; if(dy1>=0) d3=-1; if(dy3>=0) d4=-1;
			dx4=dx3; dy4=dy3; dx3=dx1; dy3=dy1; dx1=dx2; dy1=dy2;
		}else if(dx3>=0){
			t=x1; x1=x3; x3=t; y1=y2=y3;
			if(dy3<0) d1=d2=-1; if(dy2>=0) d3=-1; if(dy1>=0) d4=-1;
			dx4=dx1; dy4=dy1; dx1=dx3; dy1=dy3;
			dx3=dx2; dy3=dy2; dx2=dx1; dy2=dy1;
		}
	}
	dx1=abs(dx1); dy1=abs(dy1); dx2=abs(dx2); dy2=abs(dy2);
	dx3=abs(dx3); dy3=abs(dy3); dx4=abs(dx4); dy4=abs(dy4);
	int c1=0,c2=0; tex_index3=0; tex_dy2=1048576L;
	if((x3-x1)>0) tex_dy2/=(x3-x1); int Rndx=tri_scr_x_min+ANGLE_60-x1-1;
	for(tex_x=x1; tex_x<x2; tex_x++,Rndx--){
		if(Dist[Rndx]>mod_r){
			tex_column=tex_index3>>16;
			tex_column<<=4; tex_top=y1; tex_end=y2-y1; tex_end++;
			if(tex_top<4){ tex_end+=(tex_top-4); tex_top=4; }
			if(tex_top+tex_end>124) tex_end=124-tex_top;
			if(tex_x>(tri_scr_x_min+ANGLE_60-1)) return;
			if(tex_x>=tri_scr_x_min && tex_end>0 && tex_top<124)
				TRI_COLUMN();
		}
		tex_index3+=tex_dy2;
		for(c1+=dy1; c1>dx1; c1-=dx1,y1+=d1);
		for(c2+=dy3; c2>dx3; c2-=dx3,y2+=d3);
	}
	for(c1=c2=0; tex_x<x3; tex_x++,Rndx--){
		if(Dist[Rndx]>mod_r){
			tex_column=tex_index3>>16;
			tex_column<<=4; tex_top=y1; tex_end=y2-y1; tex_end++;
			if(tex_top<4){ tex_end+=(tex_top-4); tex_top=4; }
			if(tex_top+tex_end>124) tex_end=124-tex_top;
			if(tex_x>(tri_scr_x_min+ANGLE_60-1)) return;
			if(tex_x>=tri_scr_x_min && tex_end>0 && tex_top<124)
				TRI_COLUMN();
		}
		tex_index3+=tex_dy2;
		for(c1+=dy2; c1>dx2; c1-=dx2,y1+=d2);
		for(c2+=dy4; c2>dx4; c2-=dx4,y2+=d4);
	}
}

void TriFullScr(int x1,int y1,int x2,int y2,int x3,int y3){
	int dx1=x2-x1,dy1=y2-y1,dx2=x3-x2,dy2=y3-y2,dx3=x1-x3,dy3=y1-y3;
	if(dx1==0) dx1=1,x2++; if(dx2==0) dx2=1,x3++; if(dx3==0) dx3=-1,x1--;
	if( (dx1*dy2-dx2*dy1)<0 ){
		int t=dx1; dx1=-dx3; dx3=-t; t=dy1; dy1=-dy3; dy3=-t;
		t=x2; x2=x3; x3=t; t=y2; y2=y3; y3=t; dx2=-dx2; dy2=-dy2;
	}
	int dx4,dy4,d1=1,d2=1,d3=1,d4=1,t=0;
	if(dy1>0) t++; if(dy2>0) t++; if(dy3>0) t++;
	tex_dy=1048576L;
	if(t==2){
		if(dy1<0) tex_dy/=-dy1;
		else if(dy2<0) tex_dy/=-dy2;
		else if(dy3<0) tex_dy/=-dy3;
	}else{
		if(dy1>0) tex_dy/=dy1;
		else if(dy2>0) tex_dy/=dy2;
		else if(dy3>0) tex_dy/=dy3;
	}
	t=0; if(dx1>=0) t++; if(dx2>=0) t++; if(dx3>=0) t++;
	if(t==2){
		if(dx1<0){
			t=x1; x1=x2; x2=x3; x3=t; y1=y2;
			if(dy2<0) d1=-1; if(dy3<0) d2=-1; if(dy1>=0) d3=d4=-1;
			dx4=dx1; dy4=dy1; dx1=dx2; dy1=dy2;
			dx2=dx3; dy2=dy3; dx3=dx4; dy3=dy4;
		}else if(dx2<0){
			t=x1; x1=x3; x3=x2; x2=t; y1=y2=y3;
			if(dy3<0) d1=-1; if(dy1<0) d2=-1; if(dy2>=0) d3=d4=-1;
			dx4=dx2; dy4=dy2; dx2=dx1; dy2=dy1;
			dx1=dx3; dy1=dy3; dx3=dx4; dy3=dy4;
		}else if(dx3<0){
			y2=y1;
			if(dy1<0) d1=-1; if(dy2<0) d2=-1; if(dy3>=0) d3=d4=-1;
			dx4=dx3; dy4=dy3;
		}
	}else{
		if(dx1>=0){
			t=x2; x2=x3; x3=t; y2=y1;
			if(dy1<0) d1=d2=-1; if(dy3>=0) d3=-1; if(dy2>=0) d4=-1;
			dx4=dx2; dy4=dy2; dx2=dx1; dy2=dy1;
		}else if(dx2>=0){
			t=x1; x1=x2; x2=t; y1=y2;
			if(dy2<0) d1=d2=-1; if(dy1>=0) d3=-1; if(dy3>=0) d4=-1;
			dx4=dx3; dy4=dy3; dx3=dx1; dy3=dy1; dx1=dx2; dy1=dy2;
		}else if(dx3>=0){
			t=x1; x1=x3; x3=t; y1=y2=y3;
			if(dy3<0) d1=d2=-1; if(dy2>=0) d3=-1; if(dy1>=0) d4=-1;
			dx4=dx1; dy4=dy1; dx1=dx3; dy1=dy3;
			dx3=dx2; dy3=dy2; dx2=dx1; dy2=dy1;
		}
	}
	dx1=abs(dx1); dy1=abs(dy1); dx2=abs(dx2); dy2=abs(dy2);
	dx3=abs(dx3); dy3=abs(dy3); dx4=abs(dx4); dy4=abs(dy4);
	int c1=0,c2=0; tex_index3=0; tex_dy2=1048576L;
	if((x3-x1)>0) tex_dy2/=(x3-x1);
	for(tex_x=x1; tex_x<x2; tex_x++){
		tex_column=tex_index3>>16;
		tex_column<<=4; tex_top=y1; tex_end=y2-y1; tex_end++;
		if(tex_top<0){ tex_end+=tex_top; tex_top=0; }
		if(tex_top+tex_end>199) tex_end=199-tex_top;
		if(tex_x>319) return;
		if(tex_x>=0 && tex_end>0 && tex_top<199)
			TRI_COLUMN();
		tex_index3+=tex_dy2;
		for(c1+=dy1; c1>dx1; c1-=dx1,y1+=d1);
		for(c2+=dy3; c2>dx3; c2-=dx3,y2+=d3);
	}
	for(c1=c2=0; tex_x<x3; tex_x++){
		tex_column=tex_index3>>16;
		tex_column<<=4; tex_top=y1; tex_end=y2-y1; tex_end++;
		if(tex_top<0){ tex_end+=tex_top; tex_top=0; }
		if(tex_top+tex_end>199) tex_end=199-tex_top;
		if(tex_x>319) return;
		if(tex_x>=0 && tex_end>0 && tex_top<199)
			TRI_COLUMN();
		tex_index3+=tex_dy2;
		for(c1+=dy2; c1>dx2; c1-=dx2,y1+=d2);
		for(c2+=dy4; c2>dx4; c2-=dx4,y2+=d4);
	}
}

char key_pressed(){
	raw_key=0;
	while(!raw_key);
	return raw_key;
}

void i2a(unsigned long num,char *s,int len){
	register int i=len; s[i]=0;
	while(num && i>0){
		num/=10; s[--i]=_DX+'0';
	}while(i>0) s[--i]=' ';
}

void move64K(char *v,char *iz){
	asm{ push ds; les di,v; lds si,iz; mov cx,16000; cld }
	REP MOVSD;
	asm pop ds;
}

void Clear64K(char far *_Ptr_){
	asm les di,_Ptr_
	_EAX^=_EAX; _CX=16000;
	REP STOSD
}