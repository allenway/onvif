#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "malloc.h"
#include "ringList.h"

CRingList::CRingList( int max )
{
	m_BlockFlag	        = 0;
	m_RingList.max	    = max;
	m_RingList.size	    = 0;
	m_RingList.front	= NULL;
	m_RingList.rear	    = NULL;
	pthread_mutex_init( &m_MutexList, NULL );
	pthread_cond_init( &m_CondList, NULL );
}

CRingList::~CRingList() 
{
	pthread_mutex_lock( &m_MutexList );
	RING_NODE *pFront	= m_RingList.front;
	RING_NODE *pNode	= NULL;
	while ( pFront != NULL )
    {
    	pNode = pFront;
    	pFront = pNode->next;
    	FreeNode( pNode );
    }
	pthread_mutex_unlock( &m_MutexList );
	pthread_mutex_destroy( &m_MutexList );
	pthread_cond_destroy( &m_CondList );
}

void CRingList::SetMax( int max )
{
	pthread_mutex_lock( &m_MutexList );
	m_RingList.max = max;
	pthread_mutex_unlock( &m_MutexList );    
}

void CRingList::SetBlock()
{
	pthread_mutex_lock( &m_MutexList );
	m_BlockFlag = 1;
	pthread_mutex_unlock( &m_MutexList );
}

bool CRingList::IsFull()
{
	bool bRet = false;
	pthread_mutex_lock( &m_MutexList );
	if ( m_RingList.size == m_RingList.max ) bRet = true;
	pthread_mutex_unlock( &m_MutexList );
	return bRet;
}

bool CRingList::IsEmpty()
{
	bool bRet = false;
	pthread_mutex_lock( &m_MutexList );
	if ( m_RingList.size == 0 ) bRet = true;
	pthread_mutex_unlock( &m_MutexList );
	return bRet;
}

int CRingList::Get( void *data, int *len )
{
	int nRet = -1;
	pthread_mutex_lock( &m_MutexList );
	if ( m_RingList.size > 0 )
    {
    	char *dataPtr = (char *)(m_RingList.front->data);
    	int dataLen = m_RingList.front->len;
    	if ( data != NULL ) memcpy( (char *)data, dataPtr, dataLen );
    	if ( len != NULL ) *len = dataLen;
    	nRet = 0;
    }
	else if ( m_BlockFlag )
    {
    	pthread_cond_wait( &m_CondList, &m_MutexList );
    }    
	pthread_mutex_unlock( &m_MutexList );
	return nRet;
}

int CRingList::Delete()
{
	int nRet = -1;
	pthread_mutex_lock( &m_MutexList );
	nRet = DelNode();
	pthread_mutex_unlock( &m_MutexList );
	return nRet;    
}

void CRingList::Wait()
{
	pthread_mutex_lock( &m_MutexList );
	pthread_cond_wait( &m_CondList, &m_MutexList );
	pthread_mutex_unlock( &m_MutexList );
}

void CRingList::Signal()
{
	pthread_cond_signal( &m_CondList );    
}

int CRingList::Pop( void *data , int *len  )
{
	int nRet = -1;
	pthread_mutex_lock( &m_MutexList );
	if ( m_RingList.size > 0 )
    {
    	char *dataPtr = (char *)(m_RingList.front->data);
    	int dataLen = m_RingList.front->len;
    	if ( data != NULL ) memcpy( (char *)data, dataPtr, dataLen );
    	if ( len != NULL ) *len = dataLen;
    	nRet = 0;
    }
	else if ( m_BlockFlag )
    {
    	pthread_cond_wait( &m_CondList, &m_MutexList );
    }
	if ( nRet == 0 ) 
    {
    	DelNode();
    }
	pthread_mutex_unlock( &m_MutexList );
	return nRet;
}

int CRingList::Put( void *data, int len )
{
	int nRet = 0;
	pthread_mutex_lock( &m_MutexList );
	if( m_RingList.size == m_RingList.max ) nRet = DelNode();
	if( nRet == 0 )
    {
    	nRet = AddNode( data, len );
    	if( m_BlockFlag && nRet == 0 && m_RingList.size == 1 )
        {
        	pthread_cond_signal( &m_CondList );
        }
    }
	pthread_mutex_unlock( &m_MutexList );
	return nRet;
}

int CRingList::Size()
{
	int nRet = -1;
	pthread_mutex_lock( &m_MutexList );
	nRet = m_RingList.size;
	pthread_mutex_unlock( &m_MutexList );
	return nRet;    
}

/* ------------------------------------------------------------------------- */

int CRingList::AddNode( void *data, int len )
{
	int nRet = -1;
	if ( data == NULL || len <= 0 ) return nRet;
    
	RING_NODE *pNode = ( RING_NODE * )Malloc( sizeof(RING_NODE) );
	if ( pNode != NULL )
    {
    	pNode->data = ( void * )Malloc( len );
    	if ( pNode->data != NULL )
        {
        	memcpy( (char *)pNode->data, (char *)data, len );
        	pNode->len = len;
        	pNode->next = NULL;
        	if ( m_RingList.size == 0 )
            {
            	m_RingList.front = pNode;
            	m_RingList.rear = pNode;
            }
        	else
            {
            	m_RingList.rear->next = pNode;
            	m_RingList.rear = pNode;
            }
        	m_RingList.size += 1;
        	nRet = 0;
        }
    	else
        {
        	Free( pNode );
        }
    }
	return nRet;
}

int CRingList::DelNode()
{
	int nRet = -1;
	if ( m_RingList.size > 0 )
    {
    	RING_NODE *pNode = m_RingList.front;
    	if ( m_RingList.front == m_RingList.rear )
        {
        	m_RingList.front     = NULL;
        	m_RingList.rear     = NULL;
        	m_RingList.size     = 0;
        }
    	else
        {
        	m_RingList.front = pNode->next;
        	m_RingList.size -= 1;
        }
    	FreeNode( pNode );
    	nRet = 0;
    }
	return nRet;
}

void CRingList::FreeNode( RING_NODE *pNode )
{
	if ( pNode != NULL )
    {
    	Free( pNode->data );
    	Free( pNode );
    }
}

