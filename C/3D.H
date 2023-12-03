#ifndef ___3D_H___
#define ___3D_H___

#define PI2 M_PI*2.0
#define PERSP_COEF 25600L
#define MAX_SHAPES 250

#include <stdio.h>

extern int z_avg[],mod_sc,mod_lower;

struct TVector{
	int x,y,z;
	TVector(int X=0,int Y=0,int Z=0):x(X),y(Y),z(Z){}
	void Put(int X,int Y,int Z){ x=X; y=Y; z=Z; }
	void scale(){
		x=(((long)x*100L)/(long)mod_sc)>>8;
		y=(((long)y*100L)/(long)mod_sc)>>8;
		z=(((long)z*100L)/(long)mod_sc)>>8;
	}
	void lower(){ y+=mod_lower; }
	void Go(int dx,int dy){ x+=dx; y+=dy; }
	void persp(){
		long d=PERSP_COEF-(long)z;
		if(!d) d=1; d=(PERSP_COEF<<8)/d;
		x=((long)x*d)>>8; y=((long)y*d)>>8;
	}
};

struct TFacet{
	char i[3]; char n;
	TFacet(int i_0=0,int i_1=1,int i_2=2){ i[0]=i_0; i[1]=i_1; i[2]=i_2; n=0; }
	void Put(int i_0,int i_1,int i_2,int N){ i[0]=i_0; i[1]=i_1; i[2]=i_2; n=N; }
};

struct TShape3D{
	int num_vec,num_fac;
	TVector *V;
	TFacet *F;
	TShape3D():num_vec(0),num_fac(0){ V=0; F=0; }
	~TShape3D(){
		if(V) delete[] V;
		if(F) delete[] F;
	}
	void turn_y();
	void scale();
	void Go(int dx,int dy);
	void persp();
	void lower();
	void Sort();
	void Draw();
	void DrawFullScr();
	void Z_Avg(int nf){
		z_avg[nf]=(V[F[nf].i[0]].z+V[F[nf].i[1]].z+V[F[nf].i[2]].z)/3;
	}
	void copy(TShape3D &s3d);
	void GetMaxMem(){ V=new TVector[50]; F=new TFacet[100]; }
};

extern TShape3D shape[MAX_SHAPES],shape_view;
#endif