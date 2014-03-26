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

		//����Ϣ�����ж�ȡ��Ϣ��ʱ���Ȳ鿴�Ƿ��иÿͻ���  ����������Դ

		EnterCriticalSection(&MsgManage::pMsgManage->cs);
		if(!Core::pcore->IsId(msg->id))								//������¿ͻ���  ����Ӷ�ʱ��
		{
			Core::pcore->InsertNode(msg->id,addrClient);			//��������û�
		}
		else
		{
			Core::pcore->SetLine(msg->id);							//ÿ�ν�����Ϣ�������߰���������Ϣ  ��������
		}
		LeaveCriticalSection(&MsgManage::pMsgManage->cs);


		WORD rmsg=msg->typeMsg;
		switch(rmsg)			//��Ϣ����
		{
		case MSG_LOGIN:
			{
				MsgManage::pMsgManage->plogin=(Login*)msg->msg;
				cout<<"�ͻ���"<<inet_ntoa(addrClient.sin_addr)<<"��¼:..id: "<<MsgManage::pMsgManage->plogin->id<<"  ����: "<<"pwd: "<<MsgManage::pMsgManage->plogin->pwd<<endl;
				Core::pcore->UpdateStatefromID(msg->id,addrClient);
				Core::pcore->SetLine(msg->id);
				buf=(char*)msg;
				sendto(MsgManage::pMsgManage->server.serstruct.sock,
					buf,5,0,(SOCKADDR*)&addrClient,sizeof(addrClient));

			}
			break;
		case MSG_ONLINE:		//������Ϣ
			{

				//���Ѻ�������
				EnterCriticalSection(&MsgManage::pMsgManage->cs);
				Core::pcore->SetLine(msg->id);
				LeaveCriticalSection(&MsgManage::pMsgManage->cs);
				cout<<"�ͻ��ˣ�"<<msg->id<<"  ������Ϣ"<<endl;
				//
			}
			break;
		case MSG_OFFONLINE:
			{
				cout<<"�ͻ��ˣ�"<<msg->id<<"  ������Ϣ"<<endl;
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
		cout<<"������ߣ���ʱ����"<<wTimerID<<endl;
		cout<<"������"<<Core::pcore->GetCount()<<endl;
		EnterCriticalSection(&MsgManage::pMsgManage->cs);
		Node* newnode;
		Core::pcore->InitMoveNode();
		while((newnode=Core::pcore->GetpMoveNode())!=NULL)
		{
			if(newnode->Num>=2&&newnode->id!=0)			//�ýڵ��Ӧ�Ŀͻ��Ѷ���  ����Ϊ1  ����3���ʾ������������ʱ�䲻����  ����10��  �������綪��
			{
				cout<<"�ͻ���:  "<<newnode->id<<" �Ѿ�����"<<endl;
				Core::pcore->RemoveNode(newnode->id);
				continue;							//ɾ���Զ��ƶ�
			}
			else
			{
				if(newnode->id==0)
				{
					break;
				}
				cout<<"�ͻ���:"<<newnode->id<<" ����"<<endl;
				newnode->IsOnLine=false;				//���ò�����  �ȴ��ͻ����ٷ���һ����������Ϣ
				newnode->Num++;							//Num++   ��һ����������Ϣ����Ϊ1 
				Core::pcore->MoveNode();
			}

		}
		LeaveCriticalSection(&MsgManage::pMsgManage->cs);
	}
}