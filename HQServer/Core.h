#ifndef FILE_CORE
#define FILE_CORE
#include <iostream>
#include <windows.h>
#include <map>
#include <vector>
using namespace std;

struct Node
{
	int id;
	SOCKADDR_IN addr;
	bool IsOnLine;	//是否在线
	UINT Num;		//初始化0，if IsOnLine=true Num=1，else Num++;   当Num到一定的数则为断线
	Node* head;
	Node* tail;
};
class Core		//线程共享的资源 核心  维护客户端在线情况
{
public:
	Core();
	~Core();
	static Core* pcore;
	void InsertNode(int id,SOCKADDR_IN);		//插入节点
	void RemoveNode(int id);					//移除节点
	bool IsId(int id);							//查看是否有节点
	void SetLine(int);							//收到信息或心跳包 设为在线	
	void OffLine();								//设为不在线
	bool LookOnLine(int);						//根据定时器查看是否在线
	UINT LookOffLineNum(int);					//查看不在线次数
	void UpdateStatefromID(int,SOCKADDR_IN);	//更新状态
	
	WORD GetCount();
	Node* GetpMoveNode();
	void MoveNode();
	void InitMoveNode();
	HANDLE event;
	SOCKADDR_IN TempAddr;
private:
	Node *list,*pMoveNode;
	size_t Size_Node;
	bool IsListNull();
	WORD count;
	
};

#endif