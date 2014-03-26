#include "MsgQueue.h"
#include <Windows.h>
#include "Msg.h"

MsgQueue::MsgQueue()
{
	head=rear=NULL;
	memset(&msg,0,sizeof(msg));
	CountMsg=0;
	len=sizeof(msg)/sizeof(char);
}

void MsgQueue::PushMsg(char* str,SOCKADDR_IN addr)
{
	node *newnode=new node;
	for(int i=0;i<len;i++)
	{
		msg[i]=*(str+i);
	}
	newnode->next=NULL;
	newnode->addr=addr;
	strcpy(newnode->nodemsg,msg);
	CountMsg++;
	if(rear==NULL)
	{
		head=newnode;
		rear=newnode->next;
	}
	else
	{
		rear->next=newnode;
		rear=newnode;
	}
}
char* MsgQueue::PopMsg(SOCKADDR_IN &addr)
{
	void* p=head;
	strcpy(msg,head->nodemsg);
	addr=head->addr;
	head=head->next;
	if(p!=NULL)
	{
		delete p;
		p=NULL;
	}
	CountMsg--;
	return msg;
}
bool MsgQueue::MsgIsEmpty()//判断消息是否为空
{
	if(!CountMsg)
	{
		return true;
	}
	else
	{
		return false;
	}
}
size_t MsgQueue::MsgCount()
{
	return CountMsg;
}