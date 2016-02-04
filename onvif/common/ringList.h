#ifndef _RING_LIST_H
#define _RING_LIST_H

#include <pthread.h>

const int RING_LIST_MAX = 25;

typedef struct ringNode
{
	int len;
	void *data;
	struct ringNode *next;
} RING_NODE;

typedef struct ringList
{
	int max;
	int size;
	RING_NODE * front;
	RING_NODE * rear;
} RING_LIST;

//
// 环形链表类
//
class CRingList
{
public:
	CRingList( int max = RING_LIST_MAX );
    ~CRingList();
	void SetMax( int max );
	void SetBlock();
	bool IsFull();
	bool IsEmpty();
	int Get( void *data, int *len );
	int Delete();
	void Wait();
	void Signal();
	int Pop( void *data, int *len );    
	int Put( void *data, int len );
	int Size();

private:
	int AddNode( void *data, int len );
	int DelNode();
	void FreeNode( RING_NODE *pNode );

private:
	RING_LIST m_RingList;
	pthread_mutex_t m_MutexList;    
	pthread_cond_t m_CondList;
	int m_BlockFlag;
};

#endif  

