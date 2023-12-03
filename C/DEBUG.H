#ifndef ___DEBUG_H___
#define ___DEBUG_H___

#define IS_DEBUG 0

#if IS_DEBUG
	#define MSG_LEN 4096
	extern char sMsg[];
	extern int iMsg,xMsg,yMsg;
	void Msg(int n,int add=1);
	void Msg(char *msg,int add=1);
#endif
#endif