#include "c\defs.h"
#include "c\3d.h"
#include <stdio.h>
#include <stdlib.h>

// void TShape3D::turn_y(){
// void TShape3D::lower(){ for(int i=0; i<num_vec; i++) V[i].lower(); }
// void TShape3D::scale(){ for(int i=0; i<num_vec; i++) V[i].scale(); }
// void TShape3D::Go(int dx,int dy){ for(int i=0; i<num_vec; i++) V[i].Go(dx,dy); }
// void TShape3D::persp(){ for(int i=0; i<num_vec; i++) V[i].persp(); }
// void TShape3D::Draw(){
// void TShape3D::DrawFullScr(){
// void TShape3D::copy(TShape3D &s){
// inline int Fcmp(unsigned ind1,unsigned ind2){
// void Exchange(unsigned ind1,unsigned ind2){
// static void near pascal qSortF(unsigned pivotP,unsigned nElem){
// void TShape3D::Sort(){

TShape3D shape[MAX_SHAPES],shape_view; int z_avg[100];

void TShape3D::turn_y(){
   long cs=COS(mod_turn),sn=SIN(mod_turn),X,Z;
   for(int i=0; i<num_vec; i++){
      X=((long)V[i].x*cs+(long)V[i].z*sn)>>9;
      Z=(-(long)V[i].x*sn+(long)V[i].z*cs)>>9;
      V[i].x=X; V[i].z=Z;
   }
}

void TShape3D::lower(){ for(int i=0; i<num_vec; i++) V[i].lower(); }
void TShape3D::scale(){ for(int i=0; i<num_vec; i++) V[i].scale(); }
void TShape3D::Go(int dx,int dy){ for(int i=0; i<num_vec; i++) V[i].Go(dx,dy); }
void TShape3D::persp(){ for(int i=0; i<num_vec; i++) V[i].persp(); }

void TShape3D::Draw(){
   int i,I;
   Sort();
   int x1,y1,x2,y2,x3,y3;
   for(i=0; i<num_fac; i++){
      tex_ptr=pFacet+(F[i].n<<8);
      I=F[i].i[0]; x1=V[I].x; y1=V[I].y;
      I=F[i].i[1]; x2=V[I].x; y2=V[I].y;
      I=F[i].i[2]; x3=V[I].x; y3=V[I].y;
      Tri(x1,y1,x2,y2,x3,y3);
   }
}

void TShape3D::DrawFullScr(){
   int i,I;
   Sort();
   int x1,y1,x2,y2,x3,y3;
   for(i=0; i<num_fac; i++){
      tex_ptr=pFacet+(F[i].n<<8);
      I=F[i].i[0]; x1=V[I].x; y1=V[I].y;
      I=F[i].i[1]; x2=V[I].x; y2=V[I].y;
      I=F[i].i[2]; x3=V[I].x; y3=V[I].y;
      TriFullScr(x1,y1,x2,y2,x3,y3);
   }
}

void TShape3D::copy(TShape3D &s){
   num_vec=s.num_vec; num_fac=s.num_fac;
   int num_v=num_vec,num_f=num_fac;
   void *psV=(void*)s.V,*psF=(void*)s.F,*pV=(void*)V,*pF=(void*)F;
   asm{ push ds; mov cx,num_v; cld; les di,pV; lds si,psV } REP MOVSD
   asm{ mov cx,num_v } REP MOVSW
   asm { mov cx,num_f; les di,pF; lds si,psF } REP MOVSD
   asm pop ds
}
/////sort////////////////////////////////////////////////////////////////
TFacet *Facets;

inline int Fcmp(unsigned ind1,unsigned ind2){
   return z_avg[ind1]-z_avg[ind2];
}

void Exchange(unsigned ind1,unsigned ind2){
   long *p1=(long*)Facets+ind1,*p2=(long*)Facets+ind2;
   _ECX=*p1; _EAX=*p2; *p1=_EAX; *p2=_ECX;
   register int t=z_avg[ind1]; z_avg[ind1]=z_avg[ind2]; z_avg[ind2]=t;
}

static void near pascal qSortF(unsigned pivotP,unsigned nElem){
   unsigned leftP,rightP,pivotEnd,pivotTemp,leftTemp;
   unsigned lNum;
   int retval;

tailRecursion:
   if(nElem <= 2){
      if(nElem == 2){
         if(Fcmp (pivotP, rightP = pivotP+1) > 0)
            Exchange (pivotP, rightP);
      }
      return;
   }
   rightP = (nElem - 1) + pivotP;
   leftP  = (nElem >> 1) + pivotP;
// sort the pivot, left, and right elements for "median of 3"
   if(Fcmp(leftP,rightP) > 0)
      Exchange(leftP,rightP);
   if(Fcmp(leftP,pivotP) > 0)
      Exchange(leftP,pivotP);
   else if(Fcmp(pivotP,rightP) > 0)
      Exchange(pivotP,rightP);
   if(nElem == 3){
      Exchange(pivotP,leftP);
      return;
   }
//  now for the classic Hoare algorithm
   leftP = pivotEnd = pivotP+1;
   do{
      while((retval = Fcmp(leftP, pivotP)) <= 0){
         if(retval == 0){
            Exchange(leftP, pivotEnd);
            pivotEnd++;
         }
         if(leftP < rightP) leftP++;
         else goto qBreak;
      }
      while(leftP < rightP){
         if((retval = Fcmp(pivotP, rightP)) < 0)
            rightP--;
         else{
            Exchange (leftP, rightP);
            if(retval != 0){
               leftP++;
               rightP--;
            }
            break;
         }
      }
   }while(leftP<rightP);

qBreak:
   if(Fcmp(leftP, pivotP) <= 0)
      leftP = (++leftP);

   leftTemp = leftP-1;
   pivotTemp = pivotP;

   while ((pivotTemp < pivotEnd) && (leftTemp >= pivotEnd)){
      Exchange(pivotTemp, leftTemp);
      pivotTemp++;
      leftTemp--;
   }

   lNum = (leftP - pivotEnd);
   nElem = ((nElem + pivotP) - leftP);

   // Sort smaller partition first to reduce stack usage
   if(nElem < lNum){
      qSortF(leftP, nElem);
      nElem = lNum;
   }else{
      qSortF(pivotP, lNum);
      pivotP = leftP;
   }
      goto tailRecursion;
}
////////////////////////////
void TShape3D::Sort(){
   for(int i=0; i<num_fac; i++) Z_Avg(i);
#if 0
   int z_max,n_max;
   for(i=0; i<num_fac-1; i++){
      z_max=z_avg[i]; n_max=i;
      for(int j=i; j<num_fac; j++)
         if(z_avg[j]<z_max)
            z_max=z_avg[j],n_max=j;
      register long sw=*((long*)(&F[i]));
      *((long*)(&F[i]))=*((long*)(&F[n_max]));
      *((long*)(&F[n_max]))=sw;
      register int t=z_avg[i]; z_avg[i]=z_avg[n_max]; z_avg[n_max]=t;
   }
#else
   Facets=&F[0];
   qSortF(0,num_fac);
#endif
}
