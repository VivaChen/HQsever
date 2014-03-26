#include "ServerInfo.h"
#include <windows.h>
#include "Core.h"
#include <stdio.h>
#include "Msg.h"
#include <iostream>
#include "MsgQueue.h"
#include "MsgManage.h"
using namespace std;

#pragma comment(lib,"ws2_32.lib")


int main(void)
{
	MsgManage msgmanage;
	Core core;

	WORD wVersionRequested;
	WSADATA wsa;
	wVersionRequested=MAKEWORD(2,2);
	int err=WSAStartup(wVersionRequested,&wsa);

	if(err!=0)
	{
		cout<<"����汾ʧ��"<<WSAGetLastError();
		Sleep(3000);
		WSACleanup();
		return 0;
	}
	if(!msgmanage.server.StartServer())
	{
		cout<<"����ʧ��"<<endl;
	}
	_beginthread(RevMsg,0,0);
	_beginthread(SendMsg,0,0);
	UINT timeid=timeSetEvent(msgmanage.timenum,1,(LPTIMECALLBACK)TimeFunc,WORD(1),TIME_PERIODIC);
	msgmanage.timeID=timeid;
	//���߳̽�����Ϣ
	char *buf=new char[msgmanage.RecvMaxlength];
	memset(buf,0,sizeof(buf));
	int len=msgmanage.SockAddrlen;
	int length=msgmanage.RecvMaxlength;
	int bytes=msgmanage.bytes;
	SOCKADDR_IN TempAddr=msgmanage.TempAddr;
	
	while(1)
	{
		if((bytes=recvfrom(msgmanage.server.serstruct.sock,buf,length,0,(SOCKADDR*)&(TempAddr),&len))==SOCKET_ERROR)
		{
			cout<<"����ʧ��,���ж�... "<<WSAGetLastError()<<endl;
			Sleep(3000);
			return 0;
		}
		msgmanage.msgqueue.PushMsg(buf,TempAddr);		//һ����Ϣ��Ӧһ���ͻ���	
		SetEvent(Core::pcore->event);
	}
	if(buf!=NULL)
	{
		delete buf;
		buf=NULL;
	}
	if(MsgManage::pMsgManage->server.IsStartServerSocketState())
	{
		MsgManage::pMsgManage->server.Close();
	}
	if(MsgManage::pMsgManage->timeID)
	{
		timeKillEvent(MsgManage::pMsgManage->timeID);
		MsgManage::pMsgManage->timeID=0;
	}
	WSACleanup();
	return 0;
}