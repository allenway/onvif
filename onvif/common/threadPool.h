#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <pthread.h>

typedef void *(*THREAD_FUN)( void * ); 

typedef struct workNode 
{ 
    THREAD_FUN fun; 
    void *args;
    struct workNode *next;
} WORK_NODE_T;

class ClThreadPool
{
public:
	ClThreadPool(): m_Shutdown( false ), m_MaxThreadNum( 0 ), m_ThreadID( NULL ),
        	m_WorkQueueSize( 0 ), m_WorkQueueFront( NULL ), m_WorkQueueRear( NULL ) {}
    ~ClThreadPool() {}

	int Create( int threadNum );    
	int Add( THREAD_FUN fun, void *args, int argsSize );    
	int Destroy();

private:
	static void *ThreadProcess( void *args );
	void ThreadRoutine();
	WORK_NODE_T *NewWork( THREAD_FUN fun, void *args, int argsSize );
    
private:
	bool m_Shutdown;
	int m_MaxThreadNum;
	pthread_t *m_ThreadID;
	int m_WorkQueueSize;
	WORK_NODE_T *m_WorkQueueFront;
	WORK_NODE_T *m_WorkQueueRear;
	pthread_mutex_t m_MutexThread;
	pthread_cond_t 	m_CondThread;
};

#endif  

