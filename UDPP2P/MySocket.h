#pragma once
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")


class CMySocket
{
public:
	CMySocket(void);
	~CMySocket(void);
public:
	int		CreateSocket(int type,BOOL flag);
	int		UDPSend(LPVOID data,int len, char* addr,int port);
	int		UDPRecv(LPVOID data,int len, char **addr=NULL,int *nPort=NULL);
	int     SendBroadcast(LPVOID data,int len,int nPort);
	int     RecvBroadcast(LPVOID data,int len,char **addr);
	int		BindSocket(int nPort);
	int		ListenSocket(int backlog);
	int		ConnectSever(char *addr,int nPort);
	int		TCPSend(LPVOID data,int len);
	int		TCPRecv(LPVOID data,int len);
CMySocket*	AcceptSocket(char **addr=NULL,int *nPort=NULL);

private:
	SOCKET	m_socket;
	BOOL	ISINIT;
	BOOL	ISBROADCATS;
};

