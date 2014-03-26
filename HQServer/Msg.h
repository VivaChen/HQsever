#ifndef FILE_SENDMSG
#define FILE_SENDMSG

#include <Windows.h>
#include <map>
using namespace std;

#define MAX_LENGTH 1024
#define MAX_FRIEND 1000		//最大人数

void RevMsg(void* param);//接收
void SendMsg(void* param);//
bool LogonServer(void* param);//登录
void SendHeardBeat(void* param);//发送心跳包
void ChatforClient(void* param);//发送在线消息
void SendOffLineMsg(void* param);//发送离线消息
void UpFriendState(void* param);//更新好友状态
void AddFriend(void* param);
void RemoveFriend(void* param);
void WINAPI TimeFunc(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);


enum typeMsg
{
	MSG_ONLINE=0,			//心跳
	MSG_LOGIN,				//登录
	MSG_CHAT,				//会话
	MSG_READFRIEND,			//读取好友
	MSG_UPDATESTAIE,		//更新状态
	MSG_ADDFRIEND,			//添加好友
	MSG_UPDATEFRIEND,		//更新好友
	MSG_REMOVEFRIEND,		//移除好友
	MSG_OFFONLINE			//离线
};
enum FriendState
{
	STATE_OFFLINE=0,	//离线
	STATE_ONLINE=1		//在线
};



#endif