#ifndef FILE_MSGQUEUE
#define FILE_MSGQUEUE
#include <vector>
#include <Windows.h>
#include "Msg.h"
using namespace std;

struct node//ÿ����Ϣ�пͻ���ַ����Ϣ����
{
	char nodemsg[MAX_LENGTH];
	node* next;
	SOCKADDR_IN addr;
	node()
	{
		memset(&addr,0,sizeof(SOCKADDR_IN));
	}
};
class MsgQueue
{
private:
	node *head;
	node *rear;
	char msg[MAX_LENGTH];
	size_t CountMsg;
public:
	MsgQueue();
	int len;
	void PushMsg(char* str,SOCKADDR_IN sddr);		//��Ϣ���
	char* PopMsg(SOCKADDR_IN &addr);						//��Ϣ����
	bool MsgIsEmpty();
	size_t MsgCount();
};
#endif