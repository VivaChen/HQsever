#include "ServerInfo.h"
using namespace std;


std::string GetDate(const char* format)
{
	time_t tm;
	struct tm *now;
	char timebuf[20];
	time(&tm);
	now=localtime(&tm);
	strftime(timebuf,sizeof(timebuf)/sizeof(char),format,now);//��now��format��ʽ����timebuf
	return std::string(timebuf);
}
ServerInfo::ServerInfo()
{
	ServerIP="192.168.1.103";
	port=3333;
	ServerSock=INVALID_SOCKET;
	memset(&serverAddress,0,sizeof(serverAddress));
}
ServerInfo::~ServerInfo()
{
		DWORD exitCode=0;
		Close();
}
bool ServerInfo::IsStartServerSocketState()
{
	if(ServerSock!=INVALID_SOCKET)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void ServerInfo::Close()
{
	if(ServerSock!=INVALID_SOCKET)
	{
		closesocket(ServerSock);
		ServerSock=INVALID_SOCKET;
	}
}
bool ServerInfo::StartServer()//��������  ����DGRAM �� �����߳̽���
{
	bool bStart=false;
	if((ServerSock=socket(AF_INET,SOCK_DGRAM,0))==INVALID_SOCKET)
	{
		cout<<"�����׽���ʧ��"<<WSAGetLastError()<<endl;
		Sleep(3000);
		return bStart=false;
	}
	serverAddress.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_port=htons(port);
	if(bind(ServerSock,(SOCKADDR*)&serverAddress,sizeof(SOCKADDR))==SOCKET_ERROR)
	{
		cout<<"����˰��׽���ʧ��"<<WSAGetLastError()<<endl;
		Sleep(3000);
		return bStart=false;
	}
	serstruct.sock=ServerSock;
	serstruct.serverinfo=this;
	cout<<"Server "<<inet_ntoa(GeHostName())<<" �˿ڣ� "<<port<<" �����ɹ�..."<<endl;
	return bStart=true;
}
in_addr ServerInfo::GeHostName()
{
	char hostname[255];
	if(gethostname(hostname,sizeof(hostname))==SOCKET_ERROR)
	{
		cout<<"��ȡ���ط��������ʧ�� "<<WSAGetLastError()<<endl;
		Sleep(3000);
		exit(-1);
	}
	struct hostent *phe=gethostbyname(hostname);
	if(phe==0)
	{
		cout<<"����IPʧ��! "<<WSAGetLastError()<<endl;
		Sleep(3000);
		exit(-1);
	}
	struct in_addr addr;
	memcpy(&addr,phe->h_addr_list[0],sizeof(struct in_addr));
	return addr;
}

bool ServerInfo::pushClien(SOCKADDR_IN Client)	//��ӿͻ��˻���¿ͻ�����Ϣ
{
	bool IsPush=false;
	DWORD clientsocket,addsocket;
	int signClient=0;
	for(Clientit=Clientaddr.begin();Clientit<Clientaddr.end();Clientit++)
	{
		signClient++;
		clientsocket=Client.sin_addr.S_un.S_addr;
		addsocket=Clientit->sin_addr.S_un.S_addr;
		if(clientsocket==addsocket)
		{
			*Clientit=Client;
			return IsPush=true;
		}
		if(Clientit==Clientaddr.end())
		{
			Clientaddr.push_back(Client);
			return IsPush=true;
		}
	}
	return IsPush=false;
}