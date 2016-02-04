#ifndef __SEMW_H__
#define __SEMW_H__

#include <semaphore.h>

int SemInit( sem_t *pSem, int share, unsigned int value );
int SemDestroy( sem_t *pSem );
int SemGetValue( sem_t *pSem, int *value );
int SemPost( sem_t *pSem );
int SemWait( sem_t *pSem );
int SemTimedWait( sem_t *pSem, int sec, int nsec );

#endif 

