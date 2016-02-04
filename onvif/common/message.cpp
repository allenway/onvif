/********************************************************************************
**  Copyright (c) 2013, 深圳市动车电气自动化有限公司, All rights reserved.
**  author        :  sven
**  version       :  v1.0
**  date           :  2013.01.17
**  description  : 消息模块,使用hash 消息.
********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "debug.h"
#include "mutex.h"
#include "malloc.h"
#include "message.h"

CMessage::CMessage()
{
	memset( m_Node, 0, sizeof(m_Node) );
}

CMessage::~CMessage()
{
	int i;
	for ( i = 0; i < MAX_NODE; i++ )
    {
    	if( NULL != m_Node[i] )
        {
        	FreeNode(m_Node[i]);
        	m_Node[i] = NULL;
        }
    }
}

// 释放某一个节点下的链表
void CMessage::FreeNode( MSG_NODE* pNode )
{
	MSG_NODE* pPrev = NULL;
	MSG_NODE* pNext = NULL;

	if ( NULL == pNode )
    {
    	return ;
    }

	pNext = pNode;

	while ( pNext )
    {
    	pPrev = pNext;
    	pNext = pNext->next;

    	Free( pPrev->msgBuf );
    	Free( pPrev );
    }
}

int CMessage::GetNodeIndex( int msgId )
{
	return msgId % MAX_NODE;
}

int CMessage::GetMutexIndex( int msgId )
{
	return msgId % MAX_MUTEX;
}

int CMessage::Send( int msgId, char* msgBuf, int msgLen )
{
	MSG_NODE * pNode = ( MSG_NODE * )Malloc( sizeof(MSG_NODE) );
	if ( NULL == pNode ) return -1;
    
	memset( pNode, 0, sizeof(MSG_NODE) );
	pNode->msgId = msgId;
	if ( NULL == msgBuf ) msgLen = 0;
	if ( msgLen > 0 )
    {
    	pNode->msgBuf = ( char* )Malloc( msgLen );
    	if ( NULL == pNode->msgBuf )
        {
        	Free( pNode );
        	return -1;
        }
    	memcpy( pNode->msgBuf, msgBuf, msgLen );
    	pNode->msgLen = msgLen;
    }
    
	int nodeIndex = GetNodeIndex( msgId );
	int mutIndex = GetMutexIndex( msgId );

    // 上锁
	m_Mutex[mutIndex].Lock();

	if ( NULL == m_Node[nodeIndex] )
    {
    	m_Node[nodeIndex] = pNode;
    }
	else
    {
    	MSG_NODE* pNext = m_Node[nodeIndex];
    	MSG_NODE* pPrev = NULL;
    	while ( pNext )
        {
        	pPrev = pNext;
        	pNext = pNext->next;
        }
    	pPrev->next = pNode;
    }

	m_Mutex[mutIndex].Unlock();    // 解锁	
    
	return msgLen;
}

bool CMessage::Find( int msgId )
{
	int index = GetNodeIndex( msgId );

	if ( NULL == m_Node[index] )
    {
    	return false;
    }

	MSG_NODE* pNext = m_Node[index];

	while ( pNext )
    {
    	if ( msgId == pNext->msgId )
        	return true;

    	pNext = pNext->next;
    }

	return false;
}

int CMessage::Recv( int msgId, char* msgBuf, int msgLen )
{
	int ret = -1;
	int mutexIndex     = GetMutexIndex( msgId );
	int nodeIndex     = GetNodeIndex( msgId );
    
	if ( NULL == m_Node[nodeIndex] )
    {
    	return -1;
    }    
    
	m_Mutex[mutexIndex].Lock();

	MSG_NODE* pPrev = NULL;
	MSG_NODE* pNext = m_Node[nodeIndex];

	while ( pNext )
    {
    	if ( pNext->msgId == msgId )
        {
        	if ( msgBuf == NULL ) 
            {
            	msgLen = 0;
            	ret = 0;
            }
        	if ( msgLen > pNext->msgLen )
            {
            	msgLen = pNext->msgLen;
            }
        	if ( msgLen > 0 ) 
            {
            	memcpy( msgBuf, pNext->msgBuf, msgLen );
            	ret = msgLen;
            }
        	if ( NULL == pPrev )
            {
            	m_Node[nodeIndex] = pNext->next;
            }
        	else
            {
            	pPrev->next = pNext->next;
            }
        
        	Free( pNext->msgBuf );
        	Free( pNext );

        	break;
        }

    	pPrev = pNext;
    	pNext = pNext->next;
    }

	m_Mutex[mutexIndex].Unlock();
    
	return ret;
}

/* ------------------------------------------------------------------------- */

static CMessage	s_MsgService;

/*
*
*/
int MessageSend( int nMsgId, char* pMsgBuf, int nMsgLen )
{
	return s_MsgService.Send( nMsgId, pMsgBuf, nMsgLen );
}

/*
* fn: 接收消息
* 返回: 大于等于0,表示有接收到消息; 小于0,表示没有接收到消息
*/
int MessageRecv( int nMsgId, char* pMsgBuf, int nMsgLen )
{
	return s_MsgService.Recv( nMsgId, pMsgBuf, nMsgLen );
}

bool MessageFind( int nMsgId )
{
	return s_MsgService.Find( nMsgId );
}

