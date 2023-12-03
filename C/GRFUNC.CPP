#include "c\defs.h"
#include <mem.h>

// void mode(int code){ asm{ mov ax,code; int 16 } }
// void MouseShow(){
// void MouseHide(){
// void line(UNS x1, UNS y1, UNS x2, UNS y2, char color){
// void xline(UNS x1, UNS y1, UNS x2, UNS y2, char color){
// void bar(UNS x1, UNS y1, UNS x2, UNS y2, char color){

void mode(int code){ asm{ mov ax,code; int 16 } }

#define MOUSE_COLOR 14
char mouse_cur_pix[8];

void MouseShow(){
	char *p=mouse_cur_pix;
	register int x=MOUSE.x,y=MOUSE.y;
	char *g=GrPtr+(y<<8)+(y<<6)+x;
	*p++=*g; *g=MOUSE_COLOR; g++;
	*p++=*g; *g=MOUSE_COLOR; g++;
	*p++=*g; *g=MOUSE_COLOR; g+=318;
	*p++=*g; *g=MOUSE_COLOR; g++;
	*p++=*g; *g=MOUSE_COLOR; g+=319;
	*p++=*g; *g=MOUSE_COLOR; g+=2;
	*p++=*g; *g=MOUSE_COLOR; g+=321;
	*p=*g;   *g=MOUSE_COLOR;
}

void MouseHide(){
	char *p=mouse_cur_pix;
	register int x=MOUSE.x,y=MOUSE.y;
	char *g=GrPtr+(y<<8)+(y<<6)+x;
	*g=*p++; g++;
	*g=*p++; g++;
	*g=*p++; g+=318;
	*g=*p++; g++;
	*g=*p++; g+=319;
	*g=*p++; g+=2;
	*g=*p++; g+=321;
	*g=*p;
}

void line(UNS x1, UNS y1, UNS x2, UNS y2, char color){
	int t, dist, xerr=0, yerr=0, incx=0, incy=0;
	int dx=x2-x1, dy=y2-y1;
	if(dx>0) incx=1; else if(dx<0) incx=-1;
	if(dy>0) incy=1; else if(dy<0) incy=-1;
	if(dx<0) dx=-dx; if(dy<0) dy=-dy;
	dist=(dx>dy)?dx:dy;
	for(t=0; t<=dist+1; t++){
		PIXEL(x1,y1)=color;
		xerr+=dx; yerr+=dy;
		if(xerr>dist){ xerr-=dist; x1+=incx; }
		if(yerr>dist){ yerr-=dist; y1+=incy; }
	}
}

void xline(UNS x1, UNS y1, UNS x2, UNS y2, char color){
	int t, dist, xerr=0, yerr=0, incx=0, incy=0;
	int dx=x2-x1, dy=y2-y1;
	if(dx>0) incx=1; else if(dx<0) incx=-1;
	if(dy>0) incy=1; else if(dy<0) incy=-1;
	if(dx<0) dx=-dx; if(dy<0) dy=-dy;
	dist=(dx>dy)?dx:dy;
	for(t=0; t<=dist+1; t++){
		PIXEL(x1,y1)^=color;
		xerr+=dx; yerr+=dy;
		if(xerr>dist){ xerr-=dist; x1+=incx; }
		if(yerr>dist){ yerr-=dist; y1+=incy; }
	}
}

void bar(UNS x1, UNS y1, UNS x2, UNS y2, char color){
	UNS dx=x2-x1, dy=y2-y1;
	char *p=GrPtr+(y1<<8)+(y1<<6)+x1;
	while(dy--){
		memset(p,color,dx);
		p+=320;
	}
}
