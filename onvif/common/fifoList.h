#ifndef __FIFOLIST_H__
#define __FIFOLIST_H__

#include <pthread.h>

#define MAX_FIFO_NODE  100

typedef struct _fifoNode_
{
	void *data;
	struct _fifoNode_ *next;
} FIFO_NODE_T;

typedef struct _fifoList_
{
	int max;
	int size;
	FIFO_NODE_T * front;
	FIFO_NODE_T * rear;
} FIFO_LIST_T;

class CFifoList
{
public:
	CFifoList( int max = MAX_FIFO_NODE, int blockFlag = 0 );
    ~CFifoList();
	void SetMax( int max );
	void SetBlock();
	bool IsFull();
	bool IsEmpty();
	int Get( void **ppData );
	int Pop( void **ppData );
	int Delete();
	void Wait();
	void Broadcast();
	int Put( void *data, int *pSize );
	int Size();    
	int DelNodeRear();

private:
	int AddNode( void *data );
	int DelNode();
	void FreeNode( FIFO_NODE_T *pNode );

private:
	FIFO_LIST_T m_FifoList;
	pthread_mutex_t m_MutexList;    
	pthread_cond_t m_CondList;
	int m_BlockFlag;
};

#endif  

