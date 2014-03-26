#ifndef  FILE_MSGMANAGE
#define  FILE_MSGMANAGE
#include "Core.h"
#include "MsgQueue.h"
#include "ServerInfo.h"

struct Msg
{
	WORD typeMsg;
	WORD id;
	char msg[MAX_LENGTH];
};

struct Login	//��¼
{
	char id[20];
	char pwd[20];
};
struct HeardBeat	//������
{
	int heat;
};

struct RFriend	//��ȡ����
{
	char *Iduser[MAX_FRIEND];
	char *Idname[MAX_FRIEND];
	int *IdState[MAX_FRIEND];
	SOCKADDR_IN *Idaddr[MAX_FRIEND];
};
struct UpfriendState	//���º���״̬
{
	char id[20];
	char name[20];
	int friendstate;
	SOCKADDR_IN addr;
};
struct LineMsg				//������Ϣ
{
	char msg[MAX_LENGTH];
};
struct OffLineMsg			//������Ϣ
{
	char id[20];
	char msg[MAX_LENGTH];
};
struct addfriendId
{
	char id[20];
};
struct Removefriend
{
	char id[20];
};
class MsgManage
{
public:
	MsgManage();
	~MsgManage();

public:
	static MsgManage* pMsgManage;
	CRITICAL_SECTION    cs;
	ServerInfo		server;
	MsgQueue		msgqueue;
public:
	SOCKADDR_IN TempAddr;
	SOCKADDR_IN ServerAddr;
	int RecvMaxlength;
	int bytes;
	UINT timeID;
	int timenum; 
	int SockAddrlen;

	HeardBeat		heardbeat,*pheardbeat;
	Login			login,*plogin;
	UpfriendState	upfriendstate,*pupfriendstate;
	LineMsg			linemsg,*plinemsg;
	OffLineMsg		offlinemsg,*pofflinemsg;
	addfriendId		addrfriend,*paddrfriend;
	Removefriend	removefriend,*removefri;
	
};

#endif