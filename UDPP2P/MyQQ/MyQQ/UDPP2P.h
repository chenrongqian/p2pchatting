#ifndef _UDP_P2P_
#define _UDP_P2P_

#define CALLBACK    __stdcall

typedef void (CALLBACK *CallRecvMsg)(char*,char*);
typedef void (CALLBACK *CallUserChange)(char*,char*,char*,char*);

#define P2PAPI extern "C" _declspec(dllexport)

P2PAPI int      InitP2P();
P2PAPI int		RegisterCallBack(CallRecvMsg,CallUserChange);
P2PAPI int		Login(char *username,char *password);
P2PAPI int		GetUserCount();
P2PAPI int		GetUserInfo(char *Head,char *Name,char *Sign);
P2PAPI int      SendMsg(char *account,char *msg);
P2PAPI int      ChangeInfo(char *Head,char *Name,char *Sign);
P2PAPI int      DemandBurrow(char *account);

#endif