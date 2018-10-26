#include "UDPP2P.h"
#include "MySocket.h"
#include <stdio.h>
#include <vector>
using namespace std;
#include <process.h>
#include <conio.h>


#define HeadOffset 0
#define NameOffset 25
#define SignOffset 50
#define AccoOffset 100

#define NaOffset 0
#define LaOffset 20
#define NpOffset 50
#define LpOffset 100

#define severaddr "139.129.29.108"
//#define severaddr "127.0.0.1"
//#define severaddr "192.168.51.106"
#define severport 6666
#define clientport 6667


CallRecvMsg RecvMsg;
CallUserChange UserInfoChange;

enum MyEnum
{
	LoginInfo = 0,
	UserMsg = 0,

	LoginOK = 1,
	Online = 1,
	CheekOK = 1,

	CheekErr = 2,
	LoginErr = 2,
	Offline = 2,

	UserChange = 3,

	CheekUser = 4,

	GetUser = 5,
	UserInfo = 5,
	DeMandBurrow = 6,

	TCPACK = 8,

	Repeat = 9
};

struct UDPData
{
	BYTE LP[20];
	BYTE WP[20];
	int port;
};

struct TCPData
{
	MyEnum TYPE;
	BYTE LP[200];
	BYTE WP[200];
};

struct UDPMsg
{
	MyEnum TYPE;
	char data[100];
};

struct UserInfo
{
	char account[20];
	char head[10];
	char name[20];
	char sign[50];
	char n_addr[20];
	int n_port;
	char l_addr[20];
	int l_port;
	int flag;
};

CMySocket m_tcp;
CMySocket m_udp;
int UDPPORT;
vector<struct UserInfo> m_vector;


void DeMandBurrowProcc(UDPMsg msg,char *addr,int port);
void DeMandBurrowProcc(TCPData data);
void UserMsgProcc(UDPMsg msg,char *addr,int port);
void RecvTCPTheard(void *lp);
void RecvUDPTheard(void *lp);

int InitP2P()
{
	int err = m_tcp.CreateSocket(SOCK_STREAM,FALSE);
	if(err !=0) return err;
	err = m_udp.CreateSocket(SOCK_DGRAM,FALSE);
	if(err !=0) return err;
	err = m_tcp.ConnectSever(severaddr,severport);
	if(err !=0) return err;
	UDPPORT = clientport;
	err = m_udp.BindSocket(UDPPORT);
	while(err != 0)
	{
		err = m_udp.BindSocket(++UDPPORT);
	}
	return err;
}

int	RegisterCallBack(CallRecvMsg CallBack1,CallUserChange CallBack2)
{
	RecvMsg = CallBack1;
	UserInfoChange = CallBack2;
	return 0;
}

int Login(char *username,char *password)
{
	char hostname[256],ip[20];
	memset(ip,0,20);
    gethostname(hostname,sizeof(hostname));  
    HOSTENT* host=gethostbyname(hostname);  
    strcpy(ip,inet_ntoa(*(in_addr*)*host->h_addr_list)); 

	TCPData data;
	memset(&data,0,sizeof(TCPData));
	data.TYPE  = MyEnum::LoginInfo;
	memcpy(data.LP,username,strlen(username));
	memcpy(data.WP,password,strlen(password));
	m_tcp.TCPSend(&data,sizeof(TCPData));
	int size = m_tcp.TCPRecv(&data,sizeof(TCPData));
	if(size <= 0) return 1;
	if(data.TYPE==MyEnum::LoginOK)
	{
		struct UserInfo user;
		memset(&user,0,sizeof(UserInfo));
		strcpy(user.account,username);
		strcpy(user.head,(char*)(data.WP+HeadOffset));
		strcpy(user.name,(char*)(data.WP+NameOffset));
		strcpy(user.sign,(char*)(data.WP+SignOffset));
		m_vector.push_back(user);
		UDPData udata;
		memset(&udata,0,sizeof(UDPData));
		udata.port = UDPPORT;
		strcpy((char*)udata.LP,username);
		strcpy((char*)udata.WP,ip);

		data.TYPE = MyEnum::CheekUser;
		
		while(1)
		{
			m_udp.UDPSend(&udata,sizeof(UDPData),severaddr,6668);
			Sleep(500);
			m_tcp.TCPSend(&data,sizeof(TCPData));
			m_tcp.TCPRecv(&data,sizeof(TCPData));
			if(data.TYPE == MyEnum::CheekOK) break;
			Sleep(1000);
		}
		return 0;
	}
	return 1;
}

int GetUserCount()
{
	TCPData data;
	data.TYPE  = MyEnum::GetUser;
	m_tcp.TCPSend(&data,sizeof(TCPData));
	_beginthread(RecvTCPTheard,0,NULL);
	_beginthread(RecvUDPTheard,0,NULL);
	return 0;
}

int GetUserInfo(char *Head,char *Name,char *Sign)
{
	strcpy(Head,m_vector[0].head);
	strcpy(Name,m_vector[0].name);
	strcpy(Sign,m_vector[0].sign);
	return 0;
}

int SendMsg(char*account,char *msg)
{
	int i;
	for(i=1; i<m_vector.size(); i++)
	{
		if(strcmp(account,m_vector[i].account)==0)
			break;
	}
	UDPMsg m_msg;
	memset(&m_msg,0,sizeof(UDPMsg));
	m_msg.TYPE = MyEnum::UserMsg;
	strcpy(m_msg.data,msg);
	switch(m_vector[i].flag)
	{
	case 0:
		m_udp.UDPSend(&m_msg,sizeof(UDPMsg),m_vector[i].n_addr,m_vector[i].n_port);
		m_udp.UDPSend(&m_msg,sizeof(UDPMsg),m_vector[i].l_addr,m_vector[i].l_port);
		break;
	case 1:
		m_udp.UDPSend(&m_msg,sizeof(UDPMsg),m_vector[i].n_addr,m_vector[i].n_port);
		break;
	case 2:
		m_udp.UDPSend(&m_msg,sizeof(UDPMsg),m_vector[i].l_addr,m_vector[i].l_port);
		break;
	}
	return 0;
}

int ChangeInfo(char *Head,char *Name,char *Sign)
{
	TCPData data;
	memset(&data,0,sizeof(TCPData));
	strcpy(m_vector[0].head,Head);
	strcpy(m_vector[0].name,Name);
	strcpy(m_vector[0].sign,Sign);
	
	strcpy((char*)(data.LP),m_vector[0].account);
	strcpy((char*)(data.WP+HeadOffset),Head);
	strcpy((char*)(data.WP+NameOffset),Name);
	strcpy((char*)(data.WP+SignOffset),Sign);
	data.TYPE = MyEnum::UserChange;
	if(m_tcp.TCPSend(&data,sizeof(TCPData))==sizeof(TCPData)) return 0;
	return 1;
}

void RecvTCPTheard(void *lp)
{
	TCPData data;
	int i,count;
	while(1)
	{
		m_tcp.TCPRecv(&data,sizeof(TCPData));
		if(data.TYPE == MyEnum::UserInfo)
		{
			struct UserInfo user;
			memset(&user,0,sizeof(UserInfo));
			strcpy(user.account,(char*)(data.WP+AccoOffset));
			strcpy(user.head,(char*)(data.WP+HeadOffset));
			strcpy(user.name,(char*)(data.WP+NameOffset));
			strcpy(user.sign,(char*)(data.WP+SignOffset));
			strcpy(user.n_addr,(char*)(data.LP+NaOffset));
			strcpy(user.l_addr,(char*)(data.LP+LaOffset));
			memcpy(&user.n_port,data.LP+NpOffset,4);
			memcpy(&user.l_port,data.LP+LpOffset,4);
			count = m_vector.size();
			for(i=0; i<count; i++)
			{
				if(strcmp(m_vector[i].account,user.account)==0)
				{
					UserInfoChange(user.account,user.head,user.name,user.sign);
					if(strlen(user.head)==0)
					{
						vector<struct UserInfo> ::iterator it;
						for(it=m_vector.begin(); it!=m_vector.end(); it++)
						{
							if(strcmp(it->account,user.account)==0)
							{
								m_vector.erase(it);
								break;
							}
						}
					}
					else
					{
						strcpy(m_vector[i].head,user.head);
						strcpy(m_vector[i].name,user.name);
						strcpy(m_vector[i].sign,user.sign);
						strcpy(m_vector[i].n_addr,user.n_addr);
						strcpy(m_vector[i].l_addr,user.l_addr);
						m_vector[i].n_port = user.n_port;
						m_vector[i].l_port = user.l_port;
					}
					break;
				}
			}
			if(i != count) continue;
			user.flag = 0;
			m_vector.push_back(user);
			UserInfoChange(user.account,user.head,user.name,user.sign);
		}
		else if(data.TYPE == MyEnum::DeMandBurrow)
		{
			DeMandBurrowProcc(data);
		}
	}
}

void RecvUDPTheard(void *lp)
{
	UDPMsg msg;
	char *addr;
	int port;
	while(1)
	{
		int len = m_udp.UDPRecv(&msg,sizeof(UDPMsg),&addr,&port);
		if(len != sizeof(UDPMsg)) continue;
		switch (msg.TYPE)
		{
		case MyEnum::DeMandBurrow:
			DeMandBurrowProcc(msg,addr,port);
			break;
		case MyEnum::UserMsg:
			UserMsgProcc(msg,addr,port);
			break;
		}
	}
}

int DemandBurrow(char *account)
{
	int i;
	for(i=1; i<m_vector.size(); i++)
	{
		if(strcmp(account,m_vector[i].account)==0)
		{
			break;
		}
	}
	if(i == m_vector.size()) return 1;
	TCPData data;
	memset(&data,0,sizeof(TCPData));
	strcpy((char*)data.LP,m_vector[0].account);
	strcpy((char*)data.WP,account);
	data.TYPE = MyEnum::DeMandBurrow;
	m_tcp.TCPSend(&data,sizeof(TCPData));
	UDPMsg msg;
	memset(&msg,0,sizeof(UDPMsg));
	msg.TYPE = MyEnum::DeMandBurrow;
	m_udp.UDPSend(&msg,sizeof(UDPMsg),m_vector[i].n_addr,m_vector[i].n_port);
	m_udp.UDPSend(&msg,sizeof(UDPMsg),m_vector[i].l_addr,m_vector[i].l_port);
	return 0;
}

void DeMandBurrowProcc(UDPMsg msg,char *addr,int port)
{
	for(int i=0; i<m_vector.size(); i++)
	{
		if(strcmp(addr,m_vector[i].n_addr)==0 && port == m_vector[i].n_port)
		{
			m_vector[i].flag = 1;
			break;
		}
		if(strcmp(addr,m_vector[i].l_addr)==0 && port == m_vector[i].l_port)
		{
			m_vector[i].flag = 2;
			break;
		}
	}
}

void DeMandBurrowProcc(TCPData data)
{
	int i;
	char account[20];
	strcpy(account,(char *)data.LP);
	for(i=1; i<m_vector.size(); i++)
	{
		if(strcmp(account,m_vector[i].account)==0)
			break;
	}
	if(i == m_vector.size()) return;
	UDPMsg msg;
	memset(&msg,0,sizeof(UDPMsg));
	msg.TYPE = MyEnum::DeMandBurrow;
	m_udp.UDPSend(&msg,sizeof(UDPMsg),m_vector[i].n_addr,m_vector[i].n_port);
	m_udp.UDPSend(&msg,sizeof(UDPMsg),m_vector[i].l_addr,m_vector[i].l_port);
}

void UserMsgProcc(UDPMsg msg,char *addr,int port)
{
	for(int i=0; i<m_vector.size(); i++)
	{
		if(strcmp(addr,m_vector[i].n_addr)==0 && port == m_vector[i].n_port)
		{
			RecvMsg(m_vector[i].account,msg.data);
			break;
		}
		if(strcmp(addr,m_vector[i].l_addr)==0 && port == m_vector[i].l_port)
		{
			RecvMsg(m_vector[i].account,msg.data);
			break;
		}
	}
}