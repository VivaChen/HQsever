#ifndef FILE_SENDMSG
#define FILE_SENDMSG

#include <Windows.h>
#include <map>
using namespace std;

#define MAX_LENGTH 1024
#define MAX_FRIEND 1000		//�������

void RevMsg(void* param);//����
void SendMsg(void* param);//
bool LogonServer(void* param);//��¼
void SendHeardBeat(void* param);//����������
void ChatforClient(void* param);//����������Ϣ
void SendOffLineMsg(void* param);//����������Ϣ
void UpFriendState(void* param);//���º���״̬
void AddFriend(void* param);
void RemoveFriend(void* param);
void WINAPI TimeFunc(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);


enum typeMsg
{
	MSG_ONLINE=0,			//����
	MSG_LOGIN,				//��¼
	MSG_CHAT,				//�Ự
	MSG_READFRIEND,			//��ȡ����
	MSG_UPDATESTAIE,		//����״̬
	MSG_ADDFRIEND,			//��Ӻ���
	MSG_UPDATEFRIEND,		//���º���
	MSG_REMOVEFRIEND,		//�Ƴ�����
	MSG_OFFONLINE			//����
};
enum FriendState
{
	STATE_OFFLINE=0,	//����
	STATE_ONLINE=1		//����
};



#endif