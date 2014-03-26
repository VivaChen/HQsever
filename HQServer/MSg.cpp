#include <iostream>
#include "ServerInfo.h"
#include <Windows.h>
#include "MsgQueue.h"
#include "MsgManage.h"
#include "Msg.h"
#include "Core.h"

using namespace std;

void RevMsg(void* param)
{
	SOCKADDR_IN addrClient;
	char* buf=new char[1024];
	InitializeCriticalSection(&MsgManage::pMsgManage->cs);
	while(1)
	{
		if(MsgManage::pMsgManage->msgqueue.MsgIsEmpty())
		{
			ResetEvent(Core::pcore->event);
		}

		WaitForSingleObject(Core::pcore->event,INFINITE);


		Msg* msg=(Msg*)MsgManage::pMsgManage->msgqueue.PopMsg(addrClient);

		//从消息队列中读取消息的时候先查看是否有该客户端  给他分配资源

		EnterCriticalSection(&MsgManage::pMsgManage->cs);
		if(!Core::pcore->IsId(msg->id))								//如果是新客户端  则添加定时器
		{
			Core::pcore->InsertNode(msg->id,addrClient);			//添加在线用户
		}
		else
		{
			Core::pcore->SetLine(msg->id);							//每次接受信息设置在线包含心跳消息  设置在线
		}
		LeaveCriticalSection(&MsgManage::pMsgManage->cs);


		WORD rmsg=msg->typeMsg;
		switch(rmsg)			//消息类型
		{
		case MSG_LOGIN:
			{
				MsgManage::pMsgManage->plogin=(Login*)msg->msg;
				cout<<"客户端"<<inet_ntoa(addrClient.sin_addr)<<"登录:..id: "<<MsgManage::pMsgManage->plogin->id<<"  密码: "<<"pwd: "<<MsgManage::pMsgManage->plogin->pwd<<endl;
				Core::pcore->UpdateStatefromID(msg->id,addrClient);
				Core::pcore->SetLine(msg->id);
				buf=(char*)msg;
				sendto(MsgManage::pMsgManage->server.serstruct.sock,
					buf,5,0,(SOCKADDR*)&addrClient,sizeof(addrClient));

			}
			break;
		case MSG_ONLINE:		//心跳消息
			{

				//提醒好友上线
				EnterCriticalSection(&MsgManage::pMsgManage->cs);
				Core::pcore->SetLine(msg->id);
				LeaveCriticalSection(&MsgManage::pMsgManage->cs);
				cout<<"客户端："<<msg->id<<"  心跳消息"<<endl;
				//
			}
			break;
		case MSG_OFFONLINE:
			{
				cout<<"客户端："<<msg->id<<"  下线消息"<<endl;
				EnterCriticalSection(&MsgManage::pMsgManage->cs);
				Core::pcore->RemoveNode(msg->id);
				LeaveCriticalSection(&MsgManage::pMsgManage->cs);
			}
			break;
		}
	}
	delete buf;
}
void SendMsg(void* param)
{
	char buf[MAX_LENGTH];
	SOCKET s=MsgManage::pMsgManage->server.serstruct.sock;
	int length=MAX_LENGTH;
	SOCKADDR_IN addr;
	memset(&addr,0,sizeof(SOCKADDR_IN));
	int len=sizeof(SOCKADDR_IN);
	while(1)
	{
		gets(buf);
		MsgManage::pMsgManage->msgqueue.PopMsg(addr);
		int err=sendto(s,
			buf,
			len,
			0,
			(SOCKADDR*)&addr,
			sizeof(SOCKADDR));
		if(err==SOCKET_ERROR)
		{
			err=WSAGetLastError();
		}
	}
}

void WINAPI TimeFunc(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2)	
{
	if(MsgManage::pMsgManage->timeID==wTimerID)
	{
		cout<<"检查在线，定时器："<<wTimerID<<endl;
		cout<<"总在线"<<Core::pcore->GetCount()<<endl;
		EnterCriticalSection(&MsgManage::pMsgManage->cs);
		Node* newnode;
		Core::pcore->InitMoveNode();
		while((newnode=Core::pcore->GetpMoveNode())!=NULL)
		{
			if(newnode->Num>=2&&newnode->id!=0)			//该节点对应的客户已断线  上线为1  大于3则表示至少两次心跳时间不在线  心跳10秒  允许网络丢包
			{
				cout<<"客户端:  "<<newnode->id<<" 已经断线"<<endl;
				Core::pcore->RemoveNode(newnode->id);
				continue;							//删除自动移动
			}
			else
			{
				if(newnode->id==0)
				{
					break;
				}
				cout<<"客户端:"<<newnode->id<<" 在线"<<endl;
				newnode->IsOnLine=false;				//设置不在线  等待客户端再发送一次心跳或信息
				newnode->Num++;							//Num++   再一次心跳或消息将设为1 
				Core::pcore->MoveNode();
			}

		}
		LeaveCriticalSection(&MsgManage::pMsgManage->cs);
	}
}