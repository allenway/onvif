#ifndef __CONDITION_H__
#define __CONDITION_H__

#include <pthread.h>
#include "mutex.h"

class ClCondition 
{
public:
	ClCondition();
    ~ClCondition();
	void Wait();
	void Signal();
	void Broadcast();

private:
	pthread_mutex_t m_mutex; 
	pthread_cond_t m_cond;
};

/********************************************************************************
**  ClCondition 与 CCondition 的区别在于
	ClCondition 已经把锁封装在wait 里面; 而CCondition 没有。
********************************************************************************/

class CCondition
{
public:
	CCondition( CMutexLock& mutex ) : m_mutex( mutex )
    {
    	pthread_cond_init( &m_cond, NULL );
    }

    ~CCondition()
    {
    	pthread_cond_destroy( &m_cond );
    }

	void Wait()
    {
    	pthread_cond_wait( &m_cond, m_mutex.GetMutex() );
    }

	void Signal()
    {
    	pthread_cond_signal( &m_cond );
    }

	void Broadcast()
    {
    	pthread_cond_broadcast( &m_cond );
    }

private:
	CMutexLock & m_mutex;
	pthread_cond_t m_cond;
};

#endif 

