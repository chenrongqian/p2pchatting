#include "MySocket.h"

CMySocket::CMySocket(void)
{
	ISINIT = FALSE;
	ISBROADCATS = FALSE;
	m_socket = 0;
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD( 2, 2 );
	
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return;
	}
	
	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */
	
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup( );
		return; 
	}
}

CMySocket::~CMySocket(void)	
{
	if(m_socket > 0)
		closesocket(m_socket);
	if(ISINIT)
		WSACleanup();
}

int CMySocket::CreateSocket(int type,BOOL flag)
{  //TCP:SOCK_STREAM UDP:SOCK_DGRAM
	m_socket = ::socket(AF_INET,type,0);

	ISBROADCATS = flag;

	if(INVALID_SOCKET == m_socket)
	{
		return WSAGetLastError();
	}
	if(!ISBROADCATS) return 0;
	BOOL bOpt = TRUE;
	setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));
	return 0;
}

int	CMySocket::BindSocket(int nPort)
{
	SOCKADDR_IN addrsrv;
	if(ISBROADCATS)
		addrsrv.sin_addr.S_un.S_addr = 0;
	else
		addrsrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrsrv.sin_family = AF_INET;
	addrsrv.sin_port = htons(nPort);
	return ::bind(m_socket,(SOCKADDR*)&addrsrv,sizeof(SOCKADDR));
}

int	CMySocket::UDPSend(LPVOID data,int len, char* addr,int port)
{
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(addr);
	addrSrv.sin_port = htons(port);
	addrSrv.sin_family = AF_INET;
	int _len = sizeof(SOCKADDR);
	return sendto(m_socket,(char *)data,len,0,(SOCKADDR*)&addrSrv,_len);
}

int	CMySocket::UDPRecv(LPVOID data,int len, char **addr,int *nPort)
{
	SOCKADDR_IN addrSrv;
	int _len = sizeof(SOCKADDR_IN);
	int recvlen = 0;
	memset(&addrSrv,0,sizeof(SOCKADDR_IN));
	recvlen = recvfrom(m_socket,(char*)data,len,0,(SOCKADDR*)&addrSrv,&_len);
	if(addr != NULL)
		*addr = inet_ntoa(addrSrv.sin_addr);
	if(nPort != NULL)
		*nPort = ntohs(addrSrv.sin_port);
	return recvlen;
}

int CMySocket::SendBroadcast(LPVOID data,int len,int nPort)
{
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(nPort);
	sin.sin_addr.s_addr = INADDR_BROADCAST;
	return sendto(m_socket, (char*)data, len, 0, (SOCKADDR*)&sin, sizeof(SOCKADDR));
}

int CMySocket::RecvBroadcast(LPVOID data,int len,char **addr)
{
	SOCKADDR_IN sin_from;
	int nAddrLen = sizeof(SOCKADDR);
	int err = recvfrom(m_socket,(char*)data,len,0,(SOCKADDR*)&sin_from,&nAddrLen);
	if(addr != NULL)
		*addr=inet_ntoa(sin_from.sin_addr);
	return err;
}

int	CMySocket::ListenSocket(int backlog)
{
	return listen(m_socket,backlog);
}

CMySocket* CMySocket::AcceptSocket(char **addr,int *nPort)
{
	CMySocket *socket = new CMySocket();
	SOCKADDR_IN sin_from;
	int nAddrLen = sizeof(SOCKADDR_IN);
	socket->m_socket = accept(m_socket,(SOCKADDR*)&sin_from,&nAddrLen);
	if(addr != NULL)
		*addr=inet_ntoa(sin_from.sin_addr);
	if(nPort != NULL)
		*nPort = ntohs(sin_from.sin_port);
	return socket;
}

int	CMySocket::ConnectSever(char *addr,int nPort)
{
	SOCKADDR_IN sin_from;
	sin_from.sin_family = AF_INET;
	sin_from.sin_port = htons(nPort);
	sin_from.sin_addr.s_addr = inet_addr(addr);
	int nAddrLen = sizeof(SOCKADDR_IN);
	return connect(m_socket,(SOCKADDR*)&sin_from,nAddrLen);
}

int	CMySocket::TCPSend(LPVOID data,int len)
{
	return send(m_socket,(char *)data,len,0);
}

int	CMySocket::TCPRecv(LPVOID data,int len)
{
	return recv(m_socket,(char *)data,len,0);
}