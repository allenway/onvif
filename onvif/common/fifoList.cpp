/********************************************************************************
**  Copyright (c) 2013, 深圳市动车电气自动化有限公司, All rights reserved.
**  author        :  sven
**  version       :  v1.0
**  date           :  2013.02.17
**  description  : 实现了fifo链表基类
********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "malloc.h"
#include "fifoList.h"
#include "debug.h"
#include "const.h"

/* ===== public ===== */
CFifoList::CFifoList( int max, int blockFlag )
{
	m_BlockFlag	        = blockFlag;
	m_FifoList.max	    = max;
	m_FifoList.size	    = 0;
	m_FifoList.front	= NULL;
	m_FifoList.rear	    = NULL;
	pthread_mutex_init( &m_MutexList, NULL );
	pthread_cond_init( &m_CondList, NULL );
}

CFifoList::~CFifoList() 
{
	FIFO_NODE_T *pFront;
	FIFO_NODE_T *pNode;
	pthread_mutex_lock( &m_MutexList );
	pFront	= m_FifoList.front;
	pNode	= NULL;
	while ( pFront != NULL )
    {
    	pNode = pFront;
    	pFront = pNode->next;
    	Free( pNode->data );
    	FreeNode( pNode );
    }
	pthread_mutex_unlock( &m_MutexList );
	pthread_mutex_destroy( &m_MutexList );
	pthread_cond_destroy( &m_CondList );
}

void CFifoList::SetMax( int max )
{
	pthread_mutex_lock( &m_MutexList );
	m_FifoList.max = max;
	pthread_mutex_unlock( &m_MutexList );    
}

void CFifoList::SetBlock()
{
	pthread_mutex_lock( &m_MutexList );
	m_BlockFlag = 1;
	pthread_mutex_unlock( &m_MutexList );
}

bool CFifoList::IsFull()
{
	bool bRet = FI_FALSE;
	pthread_mutex_lock( &m_MutexList );
	if ( m_FifoList.size == m_FifoList.max ) bRet = FI_TRUE;
	pthread_mutex_unlock( &m_MutexList );
	return bRet;
}

bool CFifoList::IsEmpty()
{
	bool bRet = FI_FALSE;
	pthread_mutex_lock( &m_MutexList );
	if ( m_FifoList.size == 0 ) bRet = FI_TRUE;
	pthread_mutex_unlock( &m_MutexList );
	return bRet;
}

int CFifoList::Get( void **ppData )
{
	int 	nRet = -1;
	pthread_mutex_lock( &m_MutexList );
	if ( m_FifoList.size > 0 && NULL != ppData )
    {
        
        *ppData = m_FifoList.front->data;
    	nRet = 0;
    }
	else if ( m_BlockFlag )
    {
    	pthread_cond_wait( &m_CondList, &m_MutexList );
    }    

	pthread_mutex_unlock( &m_MutexList );
    
	return nRet;
}

int CFifoList::Delete()
{
	int nRet;
	pthread_mutex_lock( &m_MutexList );
	nRet = DelNode();
	pthread_mutex_unlock( &m_MutexList );
	return nRet;    
}

void CFifoList::Broadcast()
{
	pthread_cond_broadcast( &m_CondList );    
}

int CFifoList::Size()
{
	int nRet;
	pthread_mutex_lock( &m_MutexList );
	nRet = m_FifoList.size;
	pthread_mutex_unlock( &m_MutexList );
	return nRet;    
}

/*
* fn: 往fifo 插入一个节点
* data: 节点数据,堆
* pSize: out, 存放函数返回后fifo 节点的数量
* 返回: -1 or 0
*/
int CFifoList::Put( void *data, int *pSize )
{
	int nRet = 0;
	pthread_mutex_lock( &m_MutexList );
	if ( m_FifoList.size == m_FifoList.max ) 
    {
    	if( NULL != m_FifoList.front->data )
        {
        	Free( m_FifoList.front->data ); // 在删除front节点前先施放它拥有的数据
        }
    	nRet = DelNode();
    	SVPrint( "warning: fifoList overflow!\r\n" );
    }
	if ( nRet == 0 )
    {
    	nRet = AddNode( data );
    	if ( m_BlockFlag && nRet == 0 )
        {
        	pthread_cond_signal( &m_CondList );
        }
    }
	if( NULL != pSize )
    {
        *pSize = m_FifoList.size;
    }
	pthread_mutex_unlock( &m_MutexList );
    
	return nRet;
}

int CFifoList::Pop( void **ppData )
{
	int 	nRet = -1;
	pthread_mutex_lock( &m_MutexList );
	if ( m_FifoList.size > 0 && NULL != ppData )
    {        
        *ppData = m_FifoList.front->data;
    	nRet = 0;        
    	DelNode();
    }
	else if ( m_BlockFlag )
    {
    	pthread_cond_wait( &m_CondList, &m_MutexList );
    }    
    
	pthread_mutex_unlock( &m_MutexList );
    
	return nRet;
}

/* ===== private ===== */
int CFifoList::AddNode( void *data )
{
	int nRet = -1;
	FIFO_NODE_T *pNode;
    
	if ( data == NULL ) 
    {
    	return nRet;
    }
    
	pNode = ( FIFO_NODE_T * )Malloc( sizeof(FIFO_NODE_T) );
	if ( pNode != NULL )
    {
    	pNode->data = data;
    	if ( pNode->data != NULL )
        {
        	pNode->next = NULL;
        	if ( m_FifoList.size == 0 )
            {
            	m_FifoList.front = pNode;
            	m_FifoList.rear = pNode;
            }
        	else
            {
            	m_FifoList.rear->next = pNode;
            	m_FifoList.rear = pNode;
            }
        	m_FifoList.size += 1;
        	nRet = 0;
        }
    	else  
        {
        	Free( pNode );
        }
    }
	return nRet;
}

/*
* fn: 删除开头的那个节点
*/
int CFifoList::DelNode()
{
	int nRet = -1;
	FIFO_NODE_T *pNode;
	if ( m_FifoList.size > 0 )
    {
    	pNode = m_FifoList.front;
    	if ( m_FifoList.front == m_FifoList.rear )
        {
        	m_FifoList.front = NULL;
        	m_FifoList.rear = NULL;
        	m_FifoList.size = 0;
        }
    	else
        {
        	m_FifoList.front = pNode->next;
        	m_FifoList.size -= 1;
        }
    	FreeNode( pNode );
    	nRet = 0;
    }
	return nRet;
}

/*
* fn: 删除结尾的那个节点, 但没有删除该节点指向的数据
*/
int CFifoList::DelNodeRear()
{
	int nRet = -1;
	FIFO_NODE_T *pNode;
	if ( m_FifoList.size > 0 )
    {
    	pNode = m_FifoList.front;
    	if ( m_FifoList.front == m_FifoList.rear )
        {
        	m_FifoList.front     = NULL;
        	m_FifoList.rear     = NULL;
        	m_FifoList.size     = 0;
        }
    	else
        {
        	while( pNode->next != m_FifoList.rear )
            {
            	pNode = pNode->next;
            }
        	m_FifoList.rear = pNode;
        	pNode = pNode->next;
        	m_FifoList.rear->next = NULL;
        	m_FifoList.size       -= 1;
        }
    	FreeNode( pNode );
    	nRet = 0;
    }
	return nRet;
}


void CFifoList::FreeNode( FIFO_NODE_T *pNode )
{
	if ( pNode != NULL )
    {
    	Free( pNode );
    }
}

