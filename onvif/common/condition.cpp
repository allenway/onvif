/********************************************************************************
**  Copyright (c) 2013, 深圳市动车电气自动化有限公司, All rights reserved.
**  author        :  sven
**  version       :  v1.0
**  date           :  2013.10.10
**  description  : 封装条件变量基类, ClCondition 和 CCondition 的区别是
	ClCondition: 带锁
	CCondition: 不带锁
********************************************************************************/

#include "condition.h" 

static void Pthread_mutex_unlock( void *arg )
{
	pthread_mutex_t *mutex = (pthread_mutex_t *)arg;
	pthread_mutex_unlock( mutex );
}

ClCondition::ClCondition()
{
	pthread_mutex_init( &m_mutex, NULL );
	pthread_cond_init( &m_cond, NULL );        
}

ClCondition::~ClCondition()
{        
	pthread_cond_destroy( &m_cond );
	pthread_mutex_destroy( &m_mutex );
}

/********************************************************************************
sven explain:
  1. pthread_cond_wait 会在阻塞前 unlock m_mutex, 返回后会lock m_mutex,
  2. 如果没有 pthread_cleanup_pop( 1 )(回调)Pthread_mutex_unlock(m_mutex),那么多线程时有可能造成死锁
  3. pthread_cleanup_push 目的是为了防止线程在pthread_cond_wait 的时候被其他线程干掉后造成死锁
  4. pthread_mutex_lock 和 pthread_cleanup_pop( 1 )(回调)Pthread_mutex_unlock(m_mutex)配对使用
*******************************************************************************/
void ClCondition::Wait()
{
	pthread_mutex_lock( &m_mutex );    
	pthread_cleanup_push( Pthread_mutex_unlock, (void *)&m_mutex );
	pthread_cond_wait( &m_cond, &m_mutex );            
	pthread_cleanup_pop( 1 );
}

void ClCondition::Signal()
{
	pthread_mutex_lock( &m_mutex );
	pthread_cond_signal( &m_cond );
	pthread_mutex_unlock( &m_mutex );
}

void ClCondition::Broadcast()
{
	pthread_mutex_lock( &m_mutex );
	pthread_cond_broadcast( &m_cond );
	pthread_mutex_unlock( &m_mutex );
}


