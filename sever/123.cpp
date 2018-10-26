#include <iostream>
#include <vector>
using namespace std;
#include <process.h>
#include "MySocket.h"

#define HeadOffset 0
#define NameOffset 25
#define SignOffset 50
#define AccoOffset 100

#define NaOffset 0
#define LaOffset 20
#define NpOffset 50
#define LpOffset 100

enum MyEnum
{
	LoginInfo = 0,
	LoginOK = 1,
	CheekOK = 1,
	CheekErr = 2,
	LoginErr = 2,
	UserChange = 3,
	CheekUser = 4,

	GetUser = 5,
	UserInfo = 5,
	DemandBurrow = 6,
	
	TCPACK = 8,
	Repeat = 9
};

struct UserInfo
{
	char account[20];
	char network_addr[20];
	int  network_port;
	char local_addr[20];
	int  local_port;
	char head[10];
	char name[20];
	char sign[50];
	CMySocket *sock;
};

struct TCPData
{
	MyEnum TYPE;
	BYTE LP[200];
	BYTE WP[200];
};

struct UDPData
{
	BYTE LP[20];
	BYTE WP[20];
	int port;
};

void TCPRecvThread(void *lp);
void UDPRecvThread(void *lp);
void DisconPross(char *name);
void LoginInfoPross(TCPData &data,CMySocket *_sock);
void UserChangePross(TCPData data);
void CheekUserPross(TCPData &data);
void GetUserPross(char *name,CMySocket *sock);
void GetUserInfo(int i,TCPData &data);
void DemandBurrowPross(TCPData data);

vector<struct UserInfo> m_vector;
CMySocket m_tcp;
CMySocket m_udp;

int main()
{
	m_tcp.CreateSocket(SOCK_STREAM,FALSE);
	m_tcp.BindSocket(6666);
	m_tcp.ListenSocket(5);
	m_udp.CreateSocket(SOCK_DGRAM,FALSE);
	m_udp.BindSocket(6668);
	_beginthread(UDPRecvThread,0,&m_udp);
	while(1)
	{
		char *addr;
		int port;
		CMySocket *socket = m_tcp.AcceptSocket(&addr,&port);
		printf("client connet addr:%s port:%d\n",addr,port);
		_beginthread(TCPRecvThread,0,socket);
	}
	return 0;
}

void UDPRecvThread(void *lp)
{
	CMySocket *socket = (CMySocket *)lp;
	UDPData udata;
	char *addr;
	char account[20];
	int port;
	while(1)
	{
		int len = socket->UDPRecv(&udata,sizeof(UDPData),&addr,&port);
		if(len == sizeof(UDPData))
		{
			for(int i=0; i<m_vector.size(); i++)
			{
				memset(account,0,20);
				strcpy(account,(char*)(udata.LP));
				if(strcmp(account,m_vector[i].account)==0)
				{
					strcpy(m_vector[i].local_addr,(char*)(udata.WP));
					strcpy(m_vector[i].network_addr,addr);
					m_vector[i].network_port = port;
					m_vector[i].local_port = udata.port;
					TCPData data;
					GetUserInfo(i,data);
					data.TYPE = UserInfo;
					for(int j=0; j<m_vector.size(); j++)
					{
						if(strcmp(account,m_vector[j].account)!=0)
						{
							m_vector[j].sock->TCPSend(&data,sizeof(TCPData));
						}
					}
				}
			}
		}
	}
}

void TCPRecvThread(void *lp)
{
	CMySocket *socket = (CMySocket *)lp;
	TCPData data;
	char _name[20];
	memset(_name,0,20);
	while(1)
	{
		int len = socket->TCPRecv(&data,sizeof(TCPData));
		if(len <= 0) break;                                             
		if(len == sizeof(TCPData))
		{
			switch(data.TYPE) 
			{
			case LoginInfo:
				strcpy(_name,(char*)data.LP);
				LoginInfoPross(data,socket);
				socket->TCPSend(&data,sizeof(TCPData));
				break;
			case UserChange:
				UserChangePross(data);
				break;
			case CheekUser:
				CheekUserPross(data);
				socket->TCPSend(&data,sizeof(TCPData));
				break;
			case GetUser:
				GetUserPross(_name,socket);
				break;
			case DemandBurrow:
				DemandBurrowPross(data);
				break;
			}
		}
	}
	DisconPross(_name);
}

void LoginInfoPross(TCPData &data,CMySocket *_sock)
{
	char chpath[MAX_PATH];  
    GetModuleFileName(NULL,(LPSTR)chpath,sizeof(chpath));
	*(strrchr(chpath,'\\')+1)='\0';
	strcat(chpath,"user.ini");
	char _password[20];
	char _name[20];
	strcpy(_password,(char*)data.WP);
	strcpy(_name,(char*)data.LP);
	char password[20];
	char name[25];
	char head[25];
	char sign[50];
	memset(password,0,20);
	memset(name,0,25);
	memset(head,0,25);
	memset(sign,0,50);
	GetPrivateProfileString(_name,"password","",password,20,chpath);
	GetPrivateProfileString(_name,"name","",name,25,chpath);
	GetPrivateProfileString(_name,"head","",head,25,chpath);
	GetPrivateProfileString(_name,"sign","",sign,50,chpath);
	if(strcmp(_password,password)==0)
	{
		data.TYPE=LoginOK;
		struct UserInfo user;
		memset(&user,0,sizeof(UserInfo));
		strcpy(user.account,_name);
		strcpy(user.head,head);
		strcpy(user.name,name);
		strcpy(user.sign,sign);

		strcpy((char *)(data.WP+HeadOffset),head);
		strcpy((char *)(data.WP+NameOffset),name);
		strcpy((char *)(data.WP+SignOffset),sign);
		
		user.sock = _sock;

		m_vector.push_back(user);

	}
	else
		data.TYPE=LoginErr;
}

void UserChangePross(TCPData data)
{
	char chpath[MAX_PATH];  
    GetModuleFileName(NULL,(LPSTR)chpath,sizeof(chpath));
	*(strrchr(chpath,'\\')+1)='\0';
	strcat(chpath,"user.ini");
	int i=0;
	char name[25];
	char head[25];
	char sign[50];
	char account[20];
	memset(account,0,20);
	memset(name,0,25);
	memset(head,0,25);
	memset(sign,0,50);
	strcpy(account,(char *)(data.LP));
	strcpy(head,(char *)(data.WP+HeadOffset));
	strcpy(name,(char *)(data.WP+NameOffset));
	strcpy(sign,(char *)(data.WP+SignOffset));
	WritePrivateProfileString(account,"head",head,chpath);
	WritePrivateProfileString(account,"name",name,chpath);
	WritePrivateProfileString(account,"sign",sign,chpath);
	
	for(i=0; i<m_vector.size();i++)
	{
		if(strcmp(m_vector[i].account,account)==0)
		{
			strcpy(m_vector[i].head,head);
			strcpy(m_vector[i].name,name);
			strcpy(m_vector[i].sign,sign);
			GetUserInfo(i,data);
			break;
		}
	}
	data.TYPE = UserInfo;
	for(i=0; i<m_vector.size(); i++)
	{
		if(strcmp(m_vector[i].account,account)!=0)
		{
			m_vector[i].sock->TCPSend(&data,sizeof(TCPData));
		}
	}
}

void CheekUserPross(TCPData &data)
{
	char account[20];
	memset(account,0,20);
	strcpy(account,(char*)data.LP);
	int count = m_vector.size();
	for(int i=0; i<count; i++)
	{
		if(strcmp(m_vector[i].account,account)==0)
		{
			if(strlen(m_vector[i].network_addr)!=0)
				data.TYPE = CheekOK;
			else
				data.TYPE = CheekErr;
			return;
		}
	}
	data.TYPE = CheekErr;
	return;
}

void DisconPross(char *name)
{
	vector<struct UserInfo>::iterator it;
	int count = m_vector.size();
	it = m_vector.begin();
	TCPData data;
	memset(&data,0,sizeof(TCPData));
	data.TYPE = UserInfo; 
	for(int i=0; i<count; i++)
	{
		if(strcmp(m_vector[i].account,name)==0)
		{
			m_vector.erase(it);
			continue;
		}
		it++;
		strcpy((char *)(data.WP+AccoOffset),name);
		m_vector[i].sock->TCPSend(&data,sizeof(TCPData));
	}
}

void GetUserPross(char *name,CMySocket *sock)
{
	for(int i=0; i<m_vector.size(); i++)
	{
		if(strcmp(m_vector[i].account,name)!=0)
		{
			TCPData data;
			GetUserInfo(i,data);
			data.TYPE = UserInfo;
			sock->TCPSend(&data,sizeof(TCPData));
			break;
		}
	}
}

void GetUserInfo(int i,TCPData &data)
{
	memset(&data,0,sizeof(TCPData));
	strcpy((char *)(data.WP+HeadOffset),m_vector[i].head);
	strcpy((char *)(data.WP+NameOffset),m_vector[i].name);
	strcpy((char *)(data.WP+SignOffset),m_vector[i].sign);
	strcpy((char *)(data.WP+AccoOffset),m_vector[i].account);
	strcpy((char *)(data.LP+NaOffset),m_vector[i].network_addr);
	strcpy((char *)(data.LP+LaOffset),m_vector[i].local_addr);
	memcpy(data.LP+NpOffset,&(m_vector[i].network_port),4);
	memcpy(data.LP+LpOffset,&(m_vector[i].local_port),4);
}

void DemandBurrowPross(TCPData data)
{
	char account[20];
	strcpy(account,(char*)data.WP);
	for(int i=0; i<m_vector.size(); i++)
	{
		if(strcmp(account,m_vector[i].account)==0)
		{
			m_vector[i].sock->TCPSend(&data,sizeof(TCPData));
			break;
		}
	}
}
