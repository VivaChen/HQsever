#include "MsgManage.h"

MsgManage* MsgManage::pMsgManage=NULL;

MsgManage::MsgManage()
{
	pMsgManage=this;
	timeID=0;
	pheardbeat=NULL;
	plogin=NULL;
	pupfriendstate=NULL;
	plinemsg=NULL;
	pofflinemsg=NULL;
	paddrfriend=NULL;
	removefri=NULL;
	memset(&TempAddr,0,sizeof(SOCKADDR_IN));
	memset(&ServerAddr,0,sizeof(SOCKADDR_IN));
	RecvMaxlength=MAX_LENGTH;
	SockAddrlen=sizeof(SOCKADDR_IN);
	bytes=0;
	timenum=10000;
	timeID=0;
}
MsgManage::~MsgManage()
{
	if(pheardbeat==NULL)
	{
		delete pheardbeat;
		pheardbeat=NULL;
	}
	if(plogin==NULL)
	{
		delete plogin;
		plogin=NULL;
	}
	if(pupfriendstate==NULL)
	{
		delete pupfriendstate;
		pupfriendstate=NULL;
	}
	if(pofflinemsg==NULL)
	{
		delete pofflinemsg;
		pofflinemsg=NULL;
	}
	if(paddrfriend==NULL)
	{
		delete paddrfriend;
		paddrfriend=NULL;
	}
	if(removefri==NULL)
	{
		delete removefri;
		removefri=NULL;
	}
}