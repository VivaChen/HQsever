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
	bool IsOnLine;	//�Ƿ�����
	UINT Num;		//��ʼ��0��if IsOnLine=true Num=1��else Num++;   ��Num��һ��������Ϊ����
	Node* head;
	Node* tail;
};
class Core		//�̹߳������Դ ����  ά���ͻ����������
{
public:
	Core();
	~Core();
	static Core* pcore;
	void InsertNode(int id,SOCKADDR_IN);		//����ڵ�
	void RemoveNode(int id);					//�Ƴ��ڵ�
	bool IsId(int id);							//�鿴�Ƿ��нڵ�
	void SetLine(int);							//�յ���Ϣ�������� ��Ϊ����	
	void OffLine();								//��Ϊ������
	bool LookOnLine(int);						//���ݶ�ʱ���鿴�Ƿ�����
	UINT LookOffLineNum(int);					//�鿴�����ߴ���
	void UpdateStatefromID(int,SOCKADDR_IN);	//����״̬
	
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