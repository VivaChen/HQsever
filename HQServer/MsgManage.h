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

struct Login	//登录
{
	char id[20];
	char pwd[20];
};
struct HeardBeat	//心跳包
{
	int heat;
};

struct RFriend	//读取好友
{
	char *Iduser[MAX_FRIEND];
	char *Idname[MAX_FRIEND];
	int *IdState[MAX_FRIEND];
	SOCKADDR_IN *Idaddr[MAX_FRIEND];
};
struct UpfriendState	//更新好友状态
{
	char id[20];
	char name[20];
	int friendstate;
	SOCKADDR_IN addr;
};
struct LineMsg				//在线消息
{
	char msg[MAX_LENGTH];
};
struct OffLineMsg			//离线消息
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