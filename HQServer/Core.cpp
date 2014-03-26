#include "Core.h"
#include <map>
#include <vector>
#include <stdio.h>
#include "Msg.h"
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")
using namespace std;

Core* Core::pcore=NULL;
Core::Core()
{
	event=CreateEvent(NULL,TRUE,FALSE,NULL);
	pcore=this;
	count=0;
	
	Size_Node=sizeof(Node);
	list=(Node*)malloc(Size_Node);
	if(!list)
	{
		cout<<"内存分配失败"<<endl;
		Sleep(3000);
		exit(1);
	}
	//	List.head=List.tail=&List;			//环链表
	list->head=NULL;						//双向链表
	list->tail=NULL;
	list->Num=0;
	memset(&TempAddr,0,sizeof(SOCKADDR_IN));
	list->addr=TempAddr;
	list->id=0;
}
Core::~Core()
{
	ResetEvent(event);
	pMoveNode=list;
	while(list)
	{
		list=pMoveNode->tail;
		delete pMoveNode;
	}
}
void Core::InsertNode(int id,SOCKADDR_IN addr)
{
	if(IsListNull())			//空数据  
	{
		list->id=id;
		list->addr=addr;
		list->IsOnLine=true;
		list->Num=1;
		count++;
		return ;
	}
	else				//新增节点
	{
		Node* newNode=(Node*)malloc(Size_Node);			//malloc比new快   都是在堆中
		newNode->id=id;
		newNode->addr=addr;
		newNode->IsOnLine=true;
		newNode->Num=1;
		newNode->tail=list;
		newNode->head=NULL;
		list->head=newNode;
		list=newNode;
		count++;
	}
}
void Core::RemoveNode(int id)
{
	Node* newnode1,*newnode2;
	newnode1=list;
	while(newnode1)
	{
		if(newnode1->id==id)
		{
			if(newnode1->tail==NULL&&newnode1->head==NULL)		//只有一个节点
			{
				newnode1->id=0;
				count--;
				return ;
			}
			else
			{
				if(!newnode1->head)
				{
					list=list->tail;
					list->head=NULL;
					if(pMoveNode)
					{
						pMoveNode=pMoveNode->tail;
					}
					delete newnode1;
					count--;
					
					return ;
				}
				else
				{
					if(pMoveNode)
					{
						pMoveNode=pMoveNode->tail;
					}
					newnode2=newnode1->head;
					newnode2->tail=newnode1->tail;
					newnode1->head=newnode2;
					delete newnode1;					//删除节点
					count--;
					return ;
				}
			}
		}
		else
		{
			newnode1=newnode1->tail;
		}
	}
}
bool Core::IsId(int id)				//查看是否有该 ID
{
	Node* newnode;
	newnode=list;
	while(newnode)
	{
		if(newnode->id==id)
		{
			return true;
		}
		newnode=newnode->tail;
	}
	return false;
}
void Core::OffLine()			//
{
	Node* newnode;
	newnode=list;
	while(newnode)
	{
		newnode->IsOnLine=false;
		newnode->Num=0;
		newnode=newnode->tail;
	}
}
void Core::SetLine(int id)				//设定在线
{
	Node* newnode;
	newnode=list;
	while(newnode)
	{
		if(newnode->id==id)
		{
			newnode->IsOnLine=true;		//设定在线
			newnode->Num=1;				//设为1
			return ;
		}
		newnode=newnode->tail;
	}
}
bool Core::LookOnLine(int id)			
{
	Node* newnode;
	newnode=list;
	while(newnode)
	{
		if(newnode->id==id)
		{
			if(newnode->IsOnLine)
			{
				return true;
			}
			else
			{
				newnode->Num++;				//未收到心跳包  +1
				return false;
			}
		}
		newnode=newnode->tail;
	}
	return false;
}
UINT Core::LookOffLineNum(int id)			//根据ID 查看不在线次数
{
	Node* newnode;
	newnode=list;
	while(newnode)
	{
		if(newnode->id==id)
		{
			return newnode->Num;
		}
		newnode=newnode->tail;
	}
	return 0;
}
void Core::UpdateStatefromID(int id,SOCKADDR_IN addr)
{
	Node* newnode;
	newnode=list;
	while(newnode)
	{
		if(newnode->id==id)
		{
			newnode->addr=addr;
			newnode->Num=1;
			newnode->IsOnLine=true;
		}
		newnode=newnode->tail;
	}
	return ;
}
bool Core::IsListNull()
{
	if(!list)
	{
		list=(Node*)malloc(Size_Node);
		list->head=NULL;
		list->tail=NULL;
		list->id=0;
		int i=0;
		return true;
	}
	if(list->id==0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
WORD Core::GetCount()
{
	return count;
}
void Core::InitMoveNode()
{
	pMoveNode=list;
}
Node* Core::GetpMoveNode()
{
	return pMoveNode;
}
void Core::MoveNode()
{
	pMoveNode=pMoveNode->tail;			//移动到下一节点
}