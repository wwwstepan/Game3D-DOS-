#include "c\obj.h"
#include "c\3d.h"
#include "c\wea.h"
#include <stdlib.h>

// TMan::TMan():TBeing(){
// void TMan::RayCast(){
// void TMan::ChangeWeap(){
// void TMan::KIR(){
// void TMan::GUN(){
// void TMan::TAZ(){
// void TMan::ROC(){
// void TMan::MIN(){
// void TMan::ExplTelep(){
// void TMan::TEL(){ if(ammo[TEL_N] && fzWeap==3) ExplTelep(); }
// void TMan::PLM(){
// void TMan::MAV(){
// void TMan::SHK(){
// void TMan::BLD(){
// void TMan::HOL(){
// void TMan::DrawOpp(){
// int TMan::ObjIsVis(int ang2obj){
// void TMan::Draw1(){
// void TMan::Draw0(){
// int TMan::Move(){
// void TMan::Step(){
// void TMan::AddMine(){
// void TMan::Agony(){

void(TMan::*Draw[2])()={TMan::Draw0,TMan::Draw1};

void(TMan::*Fire[])()={
	TMan::KIR, TMan::GUN, TMan::PLM,
	TMan::SHK, TMan::TAZ, TMan::ROC, TMan::MAV,
	TMan::MIN, TMan::BLD, TMan::HOL, TMan::TEL
};

TMan::TMan():TBeing(){
	SCR_Y_MID=59; SCR_X_MIN=MAN_SCR_X[IX_SCR[number]];
	nInfStr=nKill=pain=armor=invis=strcnt=rent=slip=radar=0;
	fzWeap=cash=w_sp=ang=fzStep=mv_angle=mv_speed=0;
	doh=0L;
	for(int i=2; i<NUM_WEAP; i++)
		ammo[i]=0;
	ammo[0]=1; ammo[1]=20; nWeap=1;
}

void TMan::RayCast(){
	int ang=angle;
	int xray=0,yray=0,next_x_cell,next_y_cell,x_bound,y_bound,
	x_delta,y_delta,casting,x_hit_type,y_hit_type;
	int cell_x,cell_y,ray;
	float xi,yi,xi_save,yi_save,dist_x,dist_y,scale;
//  1  //
	if((ang-=ANGLE_30)<0) ang=ANGLE_360+ang;
	for(ray=1; ray<=ANGLE_60; ray++){
//  2  //
		if(ang>=ANGLE_0 && ang<ANGLE_180){
			y_bound=CELL_SIZE+(y&0xFFC0); y_delta=CELL_SIZE;
			xi=inv_tan_table[ang]*(y_bound-y)+x; next_y_cell=0;
		}else{
			y_bound=(y&0xFFC0); y_delta=-CELL_SIZE;
			xi=inv_tan_table[ang]*(y_bound-y)+x; next_y_cell=-1;
		}
//  3  //
		if(ang<ANGLE_90 || ang>=ANGLE_270){
			x_bound=CELL_SIZE+(x&0xFFC0); x_delta=CELL_SIZE;
			yi=tan_table[ang]*(x_bound-x)+y; next_x_cell=0;
		}else{
			x_bound=(x&0xFFC0); x_delta=-CELL_SIZE;
			yi=tan_table[ang]*(x_bound-x)+y; next_x_cell=-1;
		}
		casting=2; xray=yray=0;
//  4  //
		while(casting){
			if(xray!=INTERSECTION_FOUND){
				cell_x=((x_bound+next_x_cell)>>6);
				cell_y=(int)yi>>6;
				if((x_hit_type=world[cell_x][cell_y])!=0){
					dist_x=(yi-y)*inv_sin_table[ang]; yi_save=yi;
					xray=INTERSECTION_FOUND; casting--;
				}else yi+=y_step[ang];
			}
//  5  //
			if(yray!=INTERSECTION_FOUND){
				cell_x=(int)xi>>6;
				cell_y=((y_bound+next_y_cell)>>6);
				if((y_hit_type=world[cell_x][cell_y])!=0){
					dist_y=(xi-x)*inv_cos_table[ang]; xi_save=xi;
					yray=INTERSECTION_FOUND; casting--;
				}else xi+=x_step[ang];
			}
			x_bound+=x_delta; y_bound+=y_delta;
		}
//  6  //
		if(dist_x<dist_y){
//			scale=cos_table[ray]*8000/(1.0+dist_x);
			scale=cos_table[ray]*12000./(1.+dist_x);
			tex_shad=dist_x; tex_n=x_hit_type-1; tex_column=yi_save;
		}else{
//			scale=cos_table[ray]*8000/(1.0+dist_y);
			scale=cos_table[ray]*12000./(1.+dist_y);
			tex_shad=dist_y; tex_n=y_hit_type-1; tex_column=xi_save;
		}
		if(tex_n>=127) tex_n=0;
		Dist[ray-1]=tex_shad; tex_shad>>=8; tex_column&=63; tex_column>>=1;
		tex_end=scale; tex_end++; tex_top=SCR_Y_MID-(tex_end>>1);
		tex_x=SCR_X_MIN+ANGLE_60-ray; tex_index=0; tex_dy=2097152L/tex_end;
		if(tex_top<4){ tex_index=4-tex_top; tex_top=4; tex_end-=tex_index; }
		if(tex_top+tex_end>124) tex_end=124-tex_top;
		tex_index2=tex_index*tex_dy; WAL_COLUMN();
//  7  //
		if(++ang>=ANGLE_360) ang=0;
	}
}

void TMan::ChangeWeap(){
	if(nWeap<0 || fzWeap || _life==2) return;
	register int i,is=0;
	for(i=0; i<NUM_WEAP; i++)
		if(ammo[i]) is=1;
	if(!is) nWeap=-1;
	else do{
		if(++nWeap>=NUM_WEAP) nWeap=0;
	}while(!ammo[nWeap]);
	fzWeap=1;
}

void TMan::KIR(){
	if(fzWeap!=(KIR_FZ-1)) return;
	x_obj=x+(COS(angle)>>3); y_obj=y+(SIN(angle)>>3);
	r_obj=24; pow_obj=POW_KIR; t_obj=EXPL_INV; sz_obj=5000; fz_obj=3;
	AddObj(new TExpl);
}

void TMan::GUN(){
	if(!ammo[GUN_N] || fzWeap!=(GUN_FZ-1)) return;
	pow_obj=POW_GUN;
	if(AddObj(new TPul))
		ammo[GUN_N]--,mv_angle=angle,mv_speed=-5;
}

void TMan::TAZ(){
	if(!ammo[TAZ_N] || _life==2 || fzWeap!=(TAZ_FZ-11)) return;
	t_obj=100;
	if(AddObj(new TTaz))
		ammo[TAZ_N]--;
}

void TMan::ROC(){
	if(!ammo[ROC_N] || fzWeap!=(ROC_FZ-5)) return;
	if(AddObj(new TMis))
		ammo[ROC_N]--,mv_angle=angle,mv_speed=-25;
}

void TMan::MIN(){
	if(!ammo[MIN_N] || fzWeap!=(MIN_FZ-3)) return;
	h_obj=-5900; t_obj=20;
	if(AddObj(new TMine))
		ammo[MIN_N]--;
}

void TMan::ExplTelep(){
	ammo[TEL_N]--; r_obj=135; pow_obj=POW_PTL; t_obj=EXPL_NMG;
	sz_obj=13333; fz_obj=8;
	AddObj(new TExpl); RndTelep();
}

void TMan::TEL(){ if(ammo[TEL_N] && fzWeap==3) ExplTelep(); }

void TMan::PLM(){
	if(!ammo[PLM_N] || _life==2 || fzWeap<=(PLM_FZ-5)) return;
	pow_obj=POW_PLM;
	ang_obj=dAngle((rand()&31)-15);
	AddObj(new TPul); --ammo[PLM_N];
	mv_speed=-12;
}

void TMan::MAV(){
	if(!ammo[MAV_N] || fzWeap!=(MAV_FZ-2)) return;
	if(AddObj(new TMav))
		ammo[MAV_N]--,mv_angle=angle,mv_speed=-30;
}

void TMan::SHK(){
	if(!ammo[SHK_N] || fzWeap!=(SHK_FZ-1)) return;
	ammo[SHK_N]--;
	mv_speed=-1;
	int nSh=(rand()&31)+(rand()&7)+22;
	x_obj=x+(COS(ang_obj)>>3);
	y_obj=y+(SIN(ang_obj)>>3);
	while(nSh--){
		ang_obj+=(rand()&31)-15-(rand()&1);
		if(ang_obj<0) ang_obj+=ANGLE_360;
		else if(ang_obj>ANGLE_360) ang_obj-=ANGLE_360;
		x_obj+=(COS(ang_obj)>>4); y_obj+=(SIN(ang_obj)>>4);
		if(x_obj<0 || y_obj<0 || x_obj>=WORLD_X_SIZE || y_obj>=WORLD_Y_SIZE) break;
		AddObj(new TShock);
	}
}

void TMan::BLD(){
	int X=((x+(COS(angle)>>3))>>6),Y=((y+(SIN(angle)>>3))>>6);
	if(!ammo[BLD_N] || fzWeap!=(BLD_FZ-1) || world[X][Y]) return;
	for(register int i=0; i<=NUM_OBJ; i++){
		int its=Obj[i]->Its();
		if((its>31) && ((Obj[i]->x>>6)==X) && ((Obj[i]->y>>6)==Y)){
			if(its==ITS_MAN){ // если мешает человек, телепортируем его
				TMan *M=(TMan*)Obj[i];
				M->RndTelep();
				M->pain=133;
				break;
			}else return;
		}
	}
	world[X][Y]=28; ammo[BLD_N]--;
}

void TMan::HOL(){
	if(!ammo[HOL_N] || fzWeap!=3) return;
	x_obj=x+(COS(angle)>>3); y_obj=y+(SIN(angle)>>3);
	if(world[x_obj>>6][y_obj>>6])
		Hol[number]->vis=hArm[number]->vis=0;
	else{
		Hol[number]->vis=!Hol[number]->vis;
		hArm[number]->vis=!hArm[number]->vis;
		Hol[number]->x=x_obj; Hol[number]->y=y_obj;
	}
}

void TMan::DrawOpp(){
	DrawMap();
	for(int i=0; i<=NUM_OBJ; i++) Obj[i]->R(*this);
	N_MAN_DRAW=number;
	for(i=sort(); i>0; i--){
		mod_r=Obj[i]->r;
		if(mod_r<32 && _life){
			if(Obj[i]->Take(*this)==TAKE){ DelObj(i); continue; }
		}
		if(mod_r<15) continue;
		int direction=_Angle(Obj[i]->x-x,Obj[i]->y-y)*(float)ANGLE_360/6.283;
		if(direction<0) direction+=ANGLE_360;
		ang=direction-angle;
		if(ang<-ANGLE_180) ang+=ANGLE_360;
		else if(ang>ANGLE_180) ang-=ANGLE_360;
		if(ang>-ANGLE_40 && ang<ANGLE_40){
			if(rent || ObjIsVis(direction)){
				Obj[i]->Sprite(direction);
				mod_lower+=2000;
				if(tex_show<2) (*this.*Draw[tex_show])();
			}
		}
	}StateInf();
}

int TMan::ObjIsVis(int ang2obj){
	int X=x,Y=y,COSa=COS(ang2obj)>>3,SINa=SIN(ang2obj)>>3;
	for(register int n=mod_r>>6; n>1; n--){
		X+=COSa; Y+=SINa;
		if(world[X>>6][Y>>6])
			return 0;
	}return 1;
}

void TMan::Draw1(){
	shape_view.copy(shape[tex_index]);
	if(mod_turn>=ANGLE_360) mod_turn-=ANGLE_360;
	else if(mod_turn<ANGLE_0) mod_turn+=ANGLE_360;
	shape_view.turn_y(); shape_view.persp();
	shape_view.lower(); shape_view.scale();
	shape_view.Go(SCR_X_MIN+ANGLE_30-ang,SCR_Y_MID);
	tri_scr_x_min=SCR_X_MIN; if(!fzDead && rent && nWeap==SHK_N) mod_r=0;
	shape_view.Draw();
}

void TMan::Draw0(){
	tex_top+=(SCR_Y_MID);
	if(tex_top>=123 || tex_top+tex_scale<4 || tex_scale<1) return;
	tex_shad=mod_r; tex_shad>>=5; tex_end=tex_scale; tex_index=0;
	if(tex_top<4){ tex_index=4-tex_top; tex_top=4; tex_end-=tex_index; }
	if(tex_top+tex_end>124) tex_end=124-tex_top;
	if(tex_end<1) return; tex_x=SCR_X_MIN+ANGLE_30-ang-(tex_scale>>1);
	int end=tex_x+tex_scale; if(end>SCR_X_MIN+ANGLE_60) end=SCR_X_MIN+ANGLE_60;
	int x_start_index=tex_x; if(tex_x<SCR_X_MIN) tex_x=SCR_X_MIN;
	x_start_index=tex_x-x_start_index; int Rndx=SCR_X_MIN+ANGLE_60-tex_x-1;
	tex_dy=2097152L/tex_scale; tex_index2=tex_index*tex_dy;
	unsigned long tex_index3=x_start_index*tex_dy;
	if(rent && nWeap==3) mod_r=0;
	for(; tex_x<end; tex_x++,tex_index3+=tex_dy)
		if(Dist[Rndx--]>mod_r)
			tex_column=tex_index3>>16,tex_column<<=5,TRN_COLUMN();
}

int TMan::Move(){
	(*this.*ManInput[CONTROL[number]])();
	if(nWeap>=0){
		x_obj=x+(COS(angle)>>4); y_obj=y+(SIN(angle)>>4);
		ang_obj=angle; own_obj=number;
		(*this.*Fire[nWeap])();
	}
	if(fzWeap && _life!=2) fzWeap--;
	if(pain) Blood((rand()&7)+11,(rand()&7)+3);
	if(fzDead){
		if(fzDead==1) Bits(8);
		if(health>0) fzDead=0;
		else{
			if(fzDead<18) fzDead++;
			if(SCR_Y_MID>8){ SCR_Y_MID--; strcnt=0; }
			else{
				if(_life) Agony();
				if(strcnt==0) strcnt=255;
				else if(strcnt>3){
					if(nEnemy<255)
						nInfStr=(nEnemy==number)?32:23;
					else nInfStr=24;
				}return OK;
			}
		}
	}else SCR_Y_MID=59+HSTEP[fzStep];
	if(speed!=0){ if(++fzStep>10) fzStep=1;
	}else{ if(HSTEP[fzStep]!=0) fzStep++; else fzStep=0; }
	if(fzDead==0){
		int add_bit=0;
		if(health<=MAX_HEALTH/10){ health++; if(!(tic&1)) add_bit=1; }
		else if(health<=MAX_HEALTH/3){ if(!(tic&3)) health++; }
		else if(health<=MAX_HEALTH){ if(!(tic&15)) health++; }
		else if(health<=MAX_HEALTH*2){ if(!(tic&7)) health--; }
		else { if(!(tic&3)) health--; }
		if(add_bit && (rand()&1)) // Если здоровья мало, капает кровь
			Blood(1+(rand()&1),(rand()&63)+27);
	}
	Step();
	if(_life>=100) if((--_life)<100) _life=1;
	int x_index=(x+(COS(angle)>>4))>>6,y_index=(y+(SIN(angle)>>4))>>6;
	char wrld=world[x_index][y_index];
	if(wrld>=128){ // Зашел в телепорт
		wrld-=128;
		int tx=tel_w_x[wrld],ty=tel_w_y[wrld];
		if(tx<1 || ty<1 || tx>=WORLD_COL-1 || ty>=WORLD_ROW-1) RndTelep();
		else Telep((tx<<6)+32,(ty<<6)+32);
	}else if(!fzWeap && nWeap==0){ // держит кирку
		int x_k=COS(angle); x_k=(x+(x_k>>4)+(x_k>>5))>>6;
		int y_k=SIN(angle); y_k=(y+(y_k>>4)+(y_k>>5))>>6;
		wrld=world[x_k][y_k];
		if(wrld>22 && wrld<128){ fzWeap=FZ_WEAP[0]; _life=2; }
	}
	return OK;
}

void TMan::Step(){
	int into=(world[x>>6][y>>6]>1);
	if(slip){
		SlipStep();
		if((slip==1) && into){ slip=2; Die(DIE_INWALL); nEnemy=255; }
	}else{
		if(into) Die(33),nEnemy=255;
		TBeing::Step();
	}
}

void TMan::AddMine(){
	if(ammo[MIN_N]){
		h_obj=2000; t_obj=40;
		if(AddObj(new TMine))
			ammo[MIN_N]--;
	}
}

void TMan::Agony(){
	speed=0;
	if(nEnemy<NUM_OPPONENT && nEnemy!=number){
		Man[nEnemy]->nInfStr=31;
		Man[nEnemy]->strcnt=100;
	}
	x_obj=x; y_obj=y; //nKill--; //не уменьшать счет, если умер.
	AddMine(); // если есть мина, ставим ее
	if(ammo[TEL_N]){
		ExplTelep(); // если есть телепорт, телепортируем труп
		AddMine();   // если есть мина, ставим ее
	}
	if(!world[x>>6][y>>6]){ // Разбросать оружие
		ang_obj=angle; n_obj=number; AddObj(new TCorpse);
		for(t_obj=1; t_obj<NUM_WEAP; t_obj++){
			if(ammo[t_obj]){
				r_obj=ammo[t_obj]; ang_obj=random(ANGLE_360);
				h_obj=-(rand()&8191)-1000; n_obj=(rand()&15)+16;
				AddObj(new TWeap); ITEMS_LEFT++;
			}
			ammo[t_obj]=0;
		}
	}
	int i,mesto=0;
	int nkill,max_nkill=0;
	for(i=0; i<=NUM_OPPONENT; i++){
		nkill=Man[i]->nKill;
		if(nkill>nKill)
			mesto++;
		if(nkill>max_nkill)
			max_nkill=nkill;
	}
	int d_nk=max_nkill-nKill;
	if(d_nk>25) d_nk=25;
	nWeap=1; ammo[1]=(++mesto)*10; armor=d_nk<<8; _life=0;
}
