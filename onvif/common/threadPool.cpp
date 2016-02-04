/********************************************************************************
**	Copyright (c) 2013, 深圳市动车电气自动化有限公司, All rights reserved.
**	author        :  sven
**	version       :  v1.0
**	date           :  2013.10.10
**	description  : 定义线程池接口
********************************************************************************/

#include <string.h>

#include "debug.h"
#include "thread.h"
#include "malloc.h"
#include "threadPool.h"

/***********************************************************************
* fn: 创建线程池
* priority,优先级, 为线程池中每个线程的优先级
* threadNum, 线程池, 为线程池中线程个数
************************************************************************/
int ClThreadPool::Create( int threadNum ) 
{
	m_MaxThreadNum = threadNum;
	m_ThreadID = ( pthread_t * )Malloc( sizeof(pthread_t)*m_MaxThreadNum );
	if ( m_ThreadID == NULL ) return -1;
    
	pthread_mutex_init( &m_MutexThread, NULL );
	pthread_cond_init( &m_CondThread, NULL );
	for ( int i = 0; i < m_MaxThreadNum; ++i )
    {
    	ThreadCreate( &m_ThreadID[i], ThreadProcess, this );
    }
	return 0;
}

/*******************************************************************************
* fn: 把要处理的函数添加到线程池
* fun: 要处理的函数
* args: fun 要用到的参数,它所指向的内容将会被复制,所以如果它指向堆, add 完后记得释放
* argsSize: args 所指向控件存放了多少有用内容
* 其他: Add 函数里面会malloc 空间,然后把args 里的内容copy 到该空间, 而该空间的释放时间
        是在函数被执行完后,由void ClThreadPool::ThreadRoutine(), 负责释放,因此定义
        THREAD_FUN fun 实体函数的时候,不能够在里面free args。
*********************************************************************************/
int ClThreadPool::Add( THREAD_FUN fun, void *args, int argsSize ) 
{ 
	WORK_NODE_T *worker;
#if 0 // 让还没有来得及处理的任务等待,所以不限制
	if( m_WorkQueueSize == m_MaxThreadNum )
    {
    	SVPrint( "ThreadPool add error: m_WorkQueueSize(%d) == m_MaxThreadNum(%d)!\r\n", 
                                                    	m_WorkQueueSize, m_MaxThreadNum );
    	return -1;        
    }
#endif
	worker = NewWork( fun, args, argsSize );
	if ( worker == NULL ) return -1;

	pthread_mutex_lock( &m_MutexThread );    
	if ( m_WorkQueueRear == NULL )
    {
    	m_WorkQueueFront = worker;
    	m_WorkQueueRear = worker;
    }
	else
    {
    	m_WorkQueueRear->next = worker;
    	m_WorkQueueRear = worker;
    }
    ++m_WorkQueueSize;
	pthread_mutex_unlock( &m_MutexThread );
	pthread_cond_signal( &m_CondThread );

    return 0;
}

int ClThreadPool::Destroy()
{
	if ( m_Shutdown || m_ThreadID == NULL ) return -1;

	m_Shutdown = true;
	pthread_cond_broadcast( &m_CondThread );
	for ( int i = 0; i < m_MaxThreadNum; ++i )
    {
    	ThreadJoin( m_ThreadID[i], NULL );
    	Print( "Thread %d is quit to work !\r\n", m_ThreadID[i] );
    }

	m_MaxThreadNum = 0;
	Free( m_ThreadID );
    
	WORK_NODE_T *worker = NULL;
	while ( m_WorkQueueFront != NULL )
    {
    	worker = m_WorkQueueFront;
    	m_WorkQueueFront = worker->next;
    	Free ( worker );
    }
	m_WorkQueueRear = NULL;
	m_WorkQueueSize = 0;
	pthread_mutex_destroy( &m_MutexThread );
	pthread_cond_destroy( &m_CondThread );
	return 0;
}

void *ClThreadPool::ThreadProcess( void *args )
{
	if ( args != NULL )
    {
    	ClThreadPool *pThis = ( ClThreadPool * )args;
    	pThis->ThreadRoutine();
    }
	return NULL;
}

void ClThreadPool::ThreadRoutine()
{
    while ( 1 )
    {
    	pthread_mutex_lock( &m_MutexThread );    
    	while ( m_WorkQueueSize == 0 && !m_Shutdown )
        {
            //Print( "Thread %u is waiting !\r\n", ThreadSelf() );
        	pthread_cond_wait( &m_CondThread, &m_MutexThread );
        }
    	if ( m_Shutdown )
        {
        	pthread_mutex_unlock( &m_MutexThread );
            //Print( "Thread %d will quit !\r\n", ThreadSelf() );
        	break; //  while ( 1 )
        }
    	Print( "Thread %d is starting to work !\r\n", ThreadSelf() );        
        
    	WORK_NODE_T *worker     = m_WorkQueueFront;
    	m_WorkQueueFront     = worker->next;
        --m_WorkQueueSize;
    	if ( m_WorkQueueSize == 0 )	m_WorkQueueRear = NULL;
    	pthread_mutex_unlock( &m_MutexThread );

        (*(worker->fun))( worker->args );
        
    	Free( worker->args );
    	Free( worker );
    }
}

WORK_NODE_T *ClThreadPool::NewWork( THREAD_FUN fun, void *args, int argsSize )
{
	WORK_NODE_T *pResult = NULL;
	WORK_NODE_T *worker = ( WORK_NODE_T* )Malloc( sizeof(WORK_NODE_T) );
	if ( worker != NULL )
    {
    	worker->next     = NULL;
    	worker->fun     = fun;
    	worker->args     = (void *)Malloc( argsSize );
    	if( NULL != worker->args )
        {
        	memcpy(worker->args, args, argsSize);
        	pResult = worker;
        }    
    	else
        {
        	Free(worker);
        }
    }
    
	return pResult;
}

