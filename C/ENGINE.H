#ifndef ___ENGINE_H___
#define ___ENGINE_H___
#include "c\debug.h"

class TOMArEngine{
	int GAME_OVER;
	unsigned long CADR_NUMBER;
	TGameOption Options;
	TManInf ManInform[3];
	char *SAVE_FILE;
private:
	void Logo();
	void Shop();
	void ChangeHealth();
	void GameOver();
	void SetArmPosition();
	void ClearStrings();
	void MainMenu(int first=1);
	void SetPal(int n);
	void AllocMem();
	void FreeMem();
	int LoadRes();
	void ____CA();
	void MakeTables();
	void swLine(UNS x1, UNS y1, UNS x2, UNS y2);
	void SwBuffOrn();
	void LoadScreen(unsigned long nScr);
	void GrowDark(int type,int dl);
	void Quit();
	void NewItem();
	void LoadWorld(int n=-1);
	int GetNumber(char *s);
	void CreateOptionsFile();
	void LoadOptions();
	void SaveGame(char far *file_name);
	void LoadGame(char *fname);
	int QuitGame();
	void Help();
	int RealyNewGame();
	void InpText(int ix,int iy,int nch,char *s);
	int InpNumber(int ix,int iy,int nch,int step,int min,int max,int &Num,int scroll=0);
	void InpMan(int ix,int iy,int nman);
	void PrintOptions(int);
	void Weapons();
	void Controls();
	void ManHealth();
	void ManScreen();
	void InformStr();
	int InputStr(char *title,char *s,int max_len);
	void SaveLoadName(int sav=1);
	void AddSavExt(char *s);
	void ShowWhyIsEndLevel();
	void AddSand();
	void Sell_Weapons();
	void MapEditor();
#if IS_DEBUG
	void Inform();
	void Chats();
	void ShowMsg();
#else
	unsigned long *exe_len;
	int CheckEXE();
#endif
public:
	TOMArEngine(){
		GAME_OVER=0; SAVE_FILE=SAVE_FILE_NAME;
		ManInform[0].New(0); ManInform[1].New(1); ManInform[2].New(2);
#if !IS_DEBUG
		extern unsigned long OMAr_exe_len;
		exe_len=new unsigned long;
		*exe_len=OMAr_exe_len;
#endif
	}
	~TOMArEngine(){
#if !IS_DEBUG
		delete exe_len;
#endif
	}
	void Run();
};
#endif