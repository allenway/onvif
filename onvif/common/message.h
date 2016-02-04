#ifndef _HASH_MESSAGE_H
#define _HASH_MESSAGE_H

#include "mutex.h"

#define MAX_NODE 20000
#define MAX_MUTEX 40

typedef enum _Message_
{
	MSG_ID_WRITE_FLASH = 1001,
	MSG_ID_CALCULATE_HDD_SIZE,
	MSG_ID_TIMER_WRITE_SYS_CONFIG,
	MSG_ID_ALARM_WAIT_LINKAGE_THREAD,
	MSG_ID_IO_ALARM_PARAM_CHANGE,    
    
	MSG_ID_DEAL_RTP_SEND_THREAD,
	MSG_ID_DEAL_AV_SEND_THREAD,
	MSG_ID_DEAL_MESSAGE_DATA_THREAD,
	MSG_ID_ALARM_MORE_THAN_LIMIT,
	MSG_ID_DEAL_TALKBACK_THREAD,
    
	MSG_ID_DEAL_TALKBACK_RECV_THREAD,
	MSG_ID_DEAL_TALKBACK_SEND_THREAD,
	MSG_ID_RESTART_NTP,
	MSG_ID_RESTART_ENC_MODEL,    
	MSG_ID_FTP_JPG_THREAD,
    
	MSG_ID_SNAP,
	MSG_ID_ICMP_APP,
	MSG_ID_DTU_APP,
	MSG_ID_MD_PARAM_CHANGE,
	Msg_ID_MD_ALARM_PARAM_CHANGE,

	MSG_ID_DTU_HYALINE,
    MSG_ID_TIMER_WRITE_SYS_LOG,  //myf 130409 1020
    MSG_ID_SYS_UPDATE,  //myf 130508 1448
    MSG_ID_SYS_UPDATE_PRO,  //myf 130508 1448
	MSG_ID_DCP_SIGNAL,

	MSG_ID_VLOSS_ALARM_PARAM_CHANGE,  //add by jerry  20130531
	MSG_ID_SHELTER_ALARM_PARAM_CHANGE,  //add by jerry  20130531
    MSG_ID_NETWORK_CHANGE,      // 用于改变网络地址后, 重启与网络传输相关的线程
    MSG_ID_REBOOT,              // 用于重启linux 系统.
} MSG_ID_EN;

typedef struct msgNode
{
	int		msgId;
	int		msgLen;
	char *	msgBuf;
    
	struct msgNode * next;
    
} MSG_NODE;


// 专门用来发送指令型的数据
typedef struct _MsgCmd_
{
	unsigned int 	cmd;            // 指令
	unsigned int 	dataLen;        // 数据长度, 信令线程会用到,例如如果是转发指令,那么会指出转发数据的长度
	unsigned short	index;            // 当给信令线程发送指令的时候,指出对象是哪个用户
	char	    	unused[2];        // 对齐
	char             *pData;            // 该指令对应的数据
} MSG_CMD_T;

class CMessage
{
public:
	CMessage();
    ~CMessage();    
	int Send( int msgId, char* msgBuf, int msgLen );    
	int Recv( int msgId, char* msgBuf, int msgLen );
	bool Find( int msgId );

private:
	MSG_NODE *m_Node[MAX_NODE];
	ClMutexLock m_Mutex[MAX_MUTEX];
	void FreeNode( MSG_NODE* pNode );
	int GetNodeIndex( int msgId );
	int GetMutexIndex( int msgId );
};

int MessageSend( int nMsgId, char* pMsgBuf = NULL, int nMsgLen = 0 );
int MessageRecv( int nMsgId, char* pMsgBuf = NULL, int nMsgLen = 0 );
bool MessageFind( int nMsgId );

#endif 

