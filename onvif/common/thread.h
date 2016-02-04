#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>

//
// 线程优先级策略
//
#define	THREAD_SCHED_POLICY		SCHED_FIFO

//
// 线程优先级定义, 优先级定义不应该高于50.
#define THREAD_PRIORITY_COMMON	30
#define THREAD_PRIORITY_WDT		40

typedef struct _ThreadMaintain_
{
	pthread_t 	id;
	int     	runFlag;
} THREAD_MAINTAIN_T;

#ifdef __cplusplus
extern "C" {
#endif

int ThreadCreate( pthread_t *threadId, void *(*threadFunc)(void*), void *arg );
int ThreadJoin( pthread_t thread, void **valuePtr );
int ThreadDetach( pthread_t thread );
void ThreadExit( void *valuePtr );
unsigned long ThreadSelf();

int ThreadCreateCommonPriority( pthread_t *threadId, void *(*threadFunc)(void*), void *arg );
int ThreadCreateSched( pthread_t *threadId, void *(*start_routine)(void*), void *arg,
                                                	int sched_policy, int sched_priority );    

#ifdef __cplusplus
}
#endif

#endif  

