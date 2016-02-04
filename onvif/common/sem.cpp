/********************************************************************************
**	Copyright (c) 2013, 深圳市动车电气自动化有限公司, All rights reserved.
**	author        :  sven
**	version       :  v1.0
**	date           :  2013.10.10
**	description  : 封装信号量接口
********************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>

#include "sem.h"

int SemInit( sem_t *pSem, int share, unsigned int value )
{
	return sem_init( pSem, share, value);
}

int SemDestroy( sem_t *pSem )
{
	return sem_destroy( pSem );
}

int SemGetValue( sem_t *pSem, int *value )
{
	return sem_getvalue( pSem, value );
}

int SemPost( sem_t *pSem )
{
	return sem_post( pSem );
}

int SemWait( sem_t *pSem )
{
	return sem_wait( pSem );
}

/*****************************************************************
*	fn: 信号量等待
*	pSem:信号量
*	sec:等待时间(秒)
*	nsec:等待时间(纳秒)
*	返回:等到信号-0;否则-1
*******************************************************************/
int SemTimedWait( sem_t *pSem, int sec, int nsec )
{
	int ret = 0;
    
	struct timeval	tv;
	struct timespec ts;

	gettimeofday( &tv, NULL );

	ts.tv_sec     = tv.tv_sec + sec;
	ts.tv_nsec     = tv.tv_usec * 1000 + nsec;

	while ( -1 == (ret = sem_timedwait(pSem, &ts)) && errno == EINTR )
    {
    	usleep(100*1000);
    	continue;       /* Restart when interrupted by handler */
    }

	return ret;        
}



