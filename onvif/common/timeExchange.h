#ifndef __TIMEEXCHANGE_H__
#define __TIMEEXCHANGE_H__

#include <sys/time.h>

#define ONE_DAY_SECOND (24*3600)

int FiTimeHumanToUtc( int year, int month, int day, int hour, int minute, int second );
void FiTimeUtcToHuman( int utc, int *pyear, int *pmonth, int *pday,
                        	int *phour, int *pminute, int *psecond );
void FiTimeUtcToWeekDay( int utc, int *weekday );
int FiTimeUtcToWeekSecond( int utc );

void NanosleepZero();
int TimeGetDatatime( char dtime[20] );
int TimeIsValidDatetime( int year, int mon, int day, int hour, int min, int sec );
int Time();
int Gettimeofday( struct timeval *tv, struct timezone *tz );

#endif 

