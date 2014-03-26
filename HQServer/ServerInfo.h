#ifndef FILE_SERVERINFO

#define FILE_SERVERINFO

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <process.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>
using namespace std;
class ServerInfo;

struct receivStruct
{
	SOCKET sock;
	ServerInfo* serverinfo;
	receivStruct():sock(INVALID_SOCKET),serverinfo(NULL){}
};
class ServerInfo//服务端信息
{
public:
	ServerInfo();
	~ServerInfo();

	bool pushClien(SOCKADDR_IN ClientSock);
	vector<SOCKADDR_IN> popClient();
	bool StartServer(void);	//启动服务
	bool IsStartServerSocketState();
	void Close();
	receivStruct serstruct;

private:
	std::string ServerIP;
	int port;
	SOCKET ServerSock;
	vector<sockaddr_in>::iterator Clientit;
	vector<sockaddr_in> Clientaddr;
	struct sockaddr_in serverAddress;
	in_addr GeHostName(void);
};
string GetDate(const char* format);

#endif