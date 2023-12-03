#ifndef ___DEFS_H___
#define ___DEFS_H___

#define NOMONSTERS_GAME 0
#define KILLERS_GAME    1
#define COMMAND_GAME    2

#define MAX_WALL 28
#define WALL_SAND 22

#define NUM_VERSION    102
#define OMAr_CODE      0x72414D4F
#define NUM_LEVELS     50
#define NUM_WEAP       11
#define MAX_NUM_OBJ    500
#define SHOP_ARMOR     NUM_WEAP
#define SHOP_RENTGEN   (NUM_WEAP+1)
#define SHOP_RADAR     (NUM_WEAP+2)
#define SHOP_LEVEL     (NUM_WEAP+3)
#define SHOP_EXIT	  	 (NUM_WEAP+4)
#define MAX_KILL       99
#define LEV_KILL       13

typedef unsigned UNS;
typedef unsigned WORD;
typedef unsigned long DWORD;
struct TObj; struct TMan; struct TArm; struct THol;

extern void mode(int code=3);
extern void MouseShow();
extern void MouseHide();
extern void line(UNS x1, UNS y1, UNS x2, UNS y2, char color);
extern void xline(UNS x1, UNS y1, UNS x2, UNS y2, char color);
extern void bar(UNS x1, UNS y1, UNS x2, UNS y2, char color);

extern void interrupt far Key_Int(...);
extern void interrupt far TimerInt(...);
extern void Error(const char* message);
extern int AddObj(TObj *New);
extern void DelObj(int i);
extern int sort();
extern void i2a(unsigned long num,char *s,int len=5);
extern void swap(char &a,char &b);
extern void swap(char &a,char &b,char &c);

extern void ClrCharAt(char,int,int);
extern void String(long,int,int);
extern void String(char*,int,int);
extern void StringCenter(char far *str,int x,int y);

extern int Angle(int x,int y);
extern float _Angle(float x,float y);
extern int Distance(int x,int y);
extern void Tri(int x1,int y1,int x2,int y2,int x3,int y3);
extern void TriFullScr(int x1,int y1,int x2,int y2,int x3,int y3);
extern int isinum(char c);
extern int isfnum(char c);
extern char key_pressed();

extern void move64K(char*,char*);
extern void Clear64K(char far *Ptr);

extern "C" void far SWBUFF();
extern "C" void far PUTCH6();
extern "C" void far PUTCH6COLORED();
extern "C" void far CLEAR();
extern "C" void far NEGATIVE();
extern "C" void far POSITIVE();
extern "C" void far WAL_COLUMN();
extern "C" void far TRN_COLUMN();
extern "C" void far TRI_COLUMN();
extern "C" void far DRAW_BACK();

#define INT(n) __emit__(0xCD,(unsigned char)n);
#define REP    __emit__(0xF3);
#define MOVSW  __emit__(0xA5);
#define STOSW  __emit__(0xAB);
#define LODSW  __emit__(0xAD);
#define MOVSD  __emit__(0x66,0xA5);
#define STOSD  __emit__(0x66,0xAB);
#define LODSD  __emit__(0x66,0xAD);

struct TOMArFooter{
	DWORD ver;
	WORD sz_wall,sz_burst,sz_facet,sz_font;
	DWORD sz_exe,sz_res,sz_screens,sz_levels,sz_lights,sz_models;
	DWORD exe_checksum;
	WORD n_levels,n_screens,n_models;
	DWORD ofs_levels,ofs_screens,ofs_lights,ofs_pic,ofs_models,ofs_footer;
	WORD szof;
	char new_str; unsigned long OMAr;
	int Check(){ return (OMAr!=OMAr_CODE || szof!=sizeof(*this)); }
};

struct TGameOption{
	int n_man,items,levels_to_go;
	UNS time_limit;
	int gamma;
	int not_used[16];
	TGameOption(){ gamma=100; }
	void Def(){
		n_man=3; items=8; levels_to_go=5; time_limit=10919U; gamma=150;
	}
	void Check(){
		if(gamma<10 || gamma>250) gamma=150;
		if(n_man>3 || n_man<1) n_man=3;
		if(items>50 || items<0) items=20;
		if(levels_to_go>99 || levels_to_go<1) levels_to_go=10;
	}
};

struct BMP_HEADER{
	unsigned typ; unsigned long size;
	unsigned rez1,rez2; unsigned long off;
	unsigned nused1,nused2,width,nused3,heigth;
	long nused4; unsigned bpp,nused5;
};

struct TSaveHeader{
	long OMAr; int new_str;
	int num_ver,gold_left,levels_to_go,levels_pass,level_type;
	UNS time_left,time_limit;
	int game_type,is_monsters;
	UNS cur_lev,prev_lev,prev2_lev,prev3_lev,next_lev;
	char lic_wea[NUM_WEAP];
	char ix_scr[3];
	int control[3];
	int man_price[3];
	int man_hlth[3];
	char killed[3];
	char hologr[3];
	int hx[3],hy[3];
	void Get(TGameOption&);
	void Load(TGameOption &O);
	int Check(){ return (num_ver!=NUM_VERSION || OMAr!=OMAr_CODE); }
};

struct TMouse{
	char b; UNS x,y; int _x,_y;
	void Get(){ _AX=3; INT(0x33); b=_BL; _CX>>=1; x=_CX; y=_DX; }
	void Set(UNS newx,UNS newy){ _AX=4; _CX=newx; _CX<<=1; _DX=newy; INT(0x33); }
	void Rel(){ _AX=0x0B; INT(0x33); _x=_CX; _y=_DX; }
	void Range(UNS rx,UNS ry){
		_DX=rx; _DX<<=1; _CX=0; _AX=7; INT(0x33);
		_DX=ry; _CX=0; _AX=8; INT(0x33); }
	int In(UNS l, UNS u, UNS r, UNS d);
	int Init(){
		_AX=0; INT(0x33); if(!_AX) Error("Mouse driver not found.");
		Set(160,100); Get(); return 1;
	}
};

struct RGB{
	char red,green,blue;
	void Put(UNS r, UNS g, UNS b){ red=r&63; green=g&63; blue=b&63; }
};

struct TKeyboard{ TKeyboard(); ~TKeyboard(); };

#define TIMER_INT     0x08
#define KEYBOARD_INT  0x09
#define KEY_BUFFER	 0x60
#define KEY_CONTROL	 0x61
#define INT_CONTROL	 0x20

#define iESC    1
#define i1      2
#define i2      3
#define i3      4
#define i4      5
#define i5      6
#define i6      7
#define i7      8
#define i8      9
#define i9      10
#define i0      11
#define iMINUS  12
#define iEQU    13
#define iBSPACE 14
#define iTAB    15
#define iQ      16
#define iW      17
#define iE      18
#define iR      19
#define iT      20
#define iY      21
#define iU      22
#define iI      23
#define iO      24
#define iP      25
#define iLKV_SK 26
#define iRKV_SK 27
#define iENTER  28
#define iCTRL   29
#define iA      30
#define iS      31
#define iD      32
#define iF      33
#define iG      34
#define iH      35
#define iJ      36
#define iK      37
#define iL      38
#define iDVOET  39
#define iDV_KV  40
#define iTILDA  41
#define iLSHIFT 42
#define i_OBR_K 43
#define iZ      44
#define iX      45
#define iC      46
#define iV      47
#define iB      48
#define iN      49
#define iM      50
#define iBOLSH  51
#define iMENSH  52
#define i_PR_K  53
#define iRSHIFT 54
#define iPRNSCR 55
#define iALT    56
#define iSPACE  57
#define iCAPS_LOCK 58
#define iF1     59
#define iF2     60
#define iF3     61
#define iF4     62
#define iF5     63
#define iF6     64
#define iF7     65
#define iF8     66
#define iF9     67
#define iF10    68
#define iNUM_LOCK 69
#define iSCROLL_LOCK 70
#define iHOME   71
#define iUP     72
#define iPGUP   73
#define iGR_MINUS 74
#define iLEFT   75
#define iRIGHT  77
#define iEND    79
#define iDOWN   80
#define iPGDN   81
#define iINSERT 82
#define iF11    87
#define iF12    88
#define iDEL    83

#define VGA_STATUS	  0x3DA
#define VGA_VSINC_MASK 0x08
#define PAL_MASK       0x3C6
#define PAL_RD	        0x3C7
#define PAL_WR	        0x3C8
#define PAL_RGB        0x3C9

#define PIXEL(x,y) GrPtr[((y)<<8)+((y)<<6)+(x)]

#define INTERSECTION_FOUND 1

#define ANGLE_0     0
#define ANGLE_1     2
#define ANGLE_3     5
#define ANGLE_7    12
#define ANGLE_10   17
#define ANGLE_15   25
#define ANGLE_20   33
#define ANGLE_30   50
#define ANGLE_40   67
#define ANGLE_45   75
#define ANGLE_60  100
#define ANGLE_70  116
#define ANGLE_80  133
#define ANGLE_90  150
#define ANGLE_100 167
#define ANGLE_120 200
#define ANGLE_135 225
#define ANGLE_150 250
#define ANGLE_180 300
#define ANGLE_210 350
#define ANGLE_225 375
#define ANGLE_240 400
#define ANGLE_270 450
#define ANGLE_300 500
#define ANGLE_315 525
#define ANGLE_330 550
#define ANGLE_360 600

enum{
	T_1AID,T_WEAP,T_ITEM,T_BARREL,T_MAN,T_PUL,T_MIS,T_LAZ,T_NAP,T_TAZ,T_BIT,
	T_GRENADE,T_EXPL,T_NEXPL,T_LIQEXPL,T_CORPSE,T_MINE,T_GOLD,T_MAV,T_SHOCK,
	T_FIRE,T_BARRELOBL,T_MONSTER,T_INCUBATOR,T_EOF=127
};

#define WORLD_ROW  64
#define WORLD_COL  64
#define LEVEL_DISK_SIZE (WORLD_ROW*WORLD_COL + 256)
#define CELL_SIZE  64
#define OVERBOARD  25
#define _OVERBOARD (CELL_SIZE-OVERBOARD)
#define WORLD_X_SIZE  (WORLD_COL * CELL_SIZE)
#define WORLD_Y_SIZE  (WORLD_ROW * CELL_SIZE)

#define NOTAKE    10
#define TAKE      11
#define STOP      12
#define DELETE    0
#define OK        1

#define ITS_EXPL      1
#define ITS_RC        30
#define ITS_INCUBATOR 31
#define ITS_THING     32
#define ITS_MAN       64

#define SCREEN_LOGO 0L
#define SCREEN_SHOP 1L
#define SCREEN_OVER 2L
#define SCREEN_MENU 3L

#define OP_NMAN    0
#define OP_GAMTYPE 1
#define OP_ITEMS   2
#define OP_TIME    3
#define OP_LEVELS  4
#define OP_GAMMA   5
#define OP_1PLAYER 6
#define OP_2PLAYER 7
#define OP_3PLAYER 8
#define OP_SAVE    9
#define OP_SCREENS 10
#define OP_CONTROL 11
#define OP_HLTH    12
#define OP_WEAPS   13
#define OP_NEWGAME 14
#define OP_BACK    15
#define OP_QUIT	 16
#define Y_OPTIONS(n) (75+((n)*7))

#define SIN(n) (SIN_TABLE[n])
#define COS(n) (SIN_TABLE[(n)+ANGLE_90])

#define GREEN_CHARS    CharPtr=pFon6+(36*17)
#define RED_CHARS      CharPtr=pFon6+(36*18)
#define WHITE_CHARS    CharPtr=pFon6+(36*19)
#define BLUE_CHARS     CharPtr=pFon6+(36*20)
#define GREEN2_CHARS   CharPtr=pFon6+(36*21)
#define MAGENTA_CHARS  CharPtr=pFon6+(36*22)

extern RGB RGB_COLOR[256];
extern TMouse MOUSE;
extern unsigned *Clock;
extern volatile unsigned tic;
extern volatile int raw_key;
extern volatile char key[];
extern char *GrArea,*GrBuff,*GrPtr,*LightLev,*CharPtr;
extern volatile UNS TIME_LEFT;
extern int GAME_IS_GO,GOLD_LEFT,ITEMS_LEFT,LEVEL_TYPE,NUM_MONS,NUM_INCUB,GAME_TYPE;
extern char world[WORLD_COL][WORLD_ROW];
extern char tel_w_x[128],tel_w_y[128];
extern float *tan_table,*inv_tan_table,*y_step,
	*x_step,*cos_table,*inv_cos_table,*inv_sin_table;
extern int *SIN_TABLE,Dist[ANGLE_60];
extern int tex_x,tex_top,tex_end,tex_index,tex_n,tex_column,
	tex_size,tex_scale,tex_shad,tex_show,mod_lower,mod_sc;
extern UNS light_seg;
extern unsigned long tex_dy,tex_index2;
extern char *tex_row,*tex_ptr;
extern char *pWall,*pBurst,*pFon6,*pFacet;
extern TMan *Man[3];
extern TArm *Arm[3];
extern THol *Hol[3];
extern TArm *hArm[3];
extern far TObj *Obj[MAX_NUM_OBJ+1];
extern char *INFO[],*STR_DTA[],*_n_;
extern char NAME[3][16];
extern char *WEAPS_NAME[];
extern int MAN_PRICE[];
extern char SAVE_FILE_NAME[13];
extern char LIC_WEA[];
extern int MAN_HLTH[3];
extern const int MAN_SCR_X[3];
extern char KILLED[3],KILL[3],IX_SCR[3],CONTROL[3];
extern const char NUM_AMMO[];
extern const int MAX_AMMO[],FZ_WEAP[],HSTEP[],PLUS_AMMO[];
extern int WEAP_PRICE_BASE[],WEAP_PRICE[];
extern int tab_angle[128][128],tab_distance[64][64];
extern int NUM_OBJ,NUM_OPPONENT,sw_buff,tri_scr_x_min,mod_turn,mod_r;
extern int items_turn_ang,x_obj,y_obj,ang_obj,t_obj,n_obj,own_obj,h_obj;
extern int pow_obj,fz_obj,r_obj,sz_obj;
extern TOMArFooter FTR;
extern char STR_LEV_PASS_LEFT[],STR_GOLD_LEFT[];
extern char STR_TIME_LEFT[],STR_MONS_LEFT[];
extern char *RES_FILE_NAME;
extern int LEVELS_PASS;
extern char *LAST_FILE_NAME;
extern char *PREV_FILE_NAME;
extern char *EXIT_FILE_SAVE;
extern int N_MAN_DRAW;
extern int IS_MONSTERS;
extern char *GAMETYPES[];
extern UNS CUR_LEV,PREV_LEV,PREV2_LEV,PREV3_LEV,NEXT_LEV;
#endif