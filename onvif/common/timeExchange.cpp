#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>  
#include <iostream>  

#include "debug.h"
#include "timeExchange.h"


//人工日期转换成 LINUX UTC 时间
int FiTimeHumanToUtc( int year, int month, int day, int hour, int minute, int second )
{
	time_t ret;
	struct tm t;
    
	t.tm_year = year-1900;
	t.tm_mon = month-1;
	t.tm_mday = day;
	t.tm_hour = hour;
	t.tm_min = minute;
	t.tm_sec = second;
	ret = mktime( &t );
	if ( ret < 0 ) 
    {
    	ERRORPRINT( "%04d-%02d-%02d %02d:%02d:%02d\r\n", year, month, day, hour, minute, second );
    	return 0;
    }
    
	return ret;
}


//LINUX UTC时间转换成人工日期
void FiTimeUtcToHuman( int utc, int *pyear, int *pmonth, int *pday,
                        	int *phour, int *pminute, int *psecond )
{
	struct timeval tv;
	struct tm tm,*ret;
    
	if ( utc < 0 )
    {
    	SVPrint( "Invalid UTC time,do not set time before 1970-1-1 00:00:00.\r\n" );
    	utc = 0;
    }
	tv.tv_sec     = utc;
	tv.tv_usec     = 0;
	ret = localtime_r( &tv.tv_sec,&tm );

	if ( pyear != NULL )    *pyear = tm.tm_year+1900;
	if ( pmonth != NULL )    *pmonth = tm.tm_mon+1;
	if ( pday != NULL )        *pday = tm.tm_mday;
	if ( phour != NULL )    *phour = tm.tm_hour;
	if ( pminute != NULL )    *pminute = tm.tm_min;
	if ( psecond != NULL )    *psecond = tm.tm_sec;
}

//LINUX UTC时间转换成一个星期的第几天
void FiTimeUtcToWeekDay( int utc, int *weekday )
{
	struct timeval tv;
	struct tm tm,*ret;
    
	if ( utc < 0 )
    {
    	SVPrint( "Invalid UTC time,do not set time before 1970-1-1 00:00:00.\r\n" );
    	utc=0;
    }
	tv.tv_sec = utc;
	tv.tv_usec = 0;
	ret=localtime_r( &tv.tv_sec, &tm );
	if ( NULL != weekday )
        *weekday = tm.tm_wday;    
}

//LINUX UTC时间转换成一个星期的第几秒
int FiTimeUtcToWeekSecond( int utc )
{
	return (utc + ONE_DAY_SECOND*4) % (ONE_DAY_SECOND*7);
}

/************************************************************************
* fn: 休眠0 秒的时间,用来多线程的时分复用调度
*************************************************************************/
void NanosleepZero()
{
	static struct timespec tszero = { 0, 0 };
	nanosleep( &tszero, NULL );
}

/***************************************************************************
* 获取linux 下data 命令显示的时间
* dtime[20]: 存放 "2013-05-18 18:28:36"
*****************************************************************************/
int TimeGetDatatime( char dtime[20] )
{  
	int ret = -1;
	struct tm *pNow;
  	struct tm tmNow = { 0 };        
	time_t tNow = time(NULL);  
    
	pNow = localtime_r( &tNow, &tmNow );    
	if( NULL != pNow )
    {
    	ret = sizeof(dtime);
    	strftime( dtime, 20, "%Y-%m-%d %H:%M:%S", &tmNow );  
    	ret = 0;
    }
	return ret;  
}

/*************************************************************************
* fn: 检查给定的时间是否合法
* 返回: 0, 非法, 1, 合法
**************************************************************************/
int TimeIsValidDatetime( int year, int mon, int day, int hour, int min, int sec )
{
	int maxDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    
	if ( ((year%4==0)&&(year%100!=0))||(year%400==0) ) 
    {
    	maxDays[1] += 1; // 闰年
    }
    
	if ( year < 1970 ) return 0;
	if ( mon < 1 || mon > 12 ) return 0;
	if ( day < 1 || day > maxDays[mon-1] ) return 0;
	if ( hour < 0 || hour > 23 ) return 0;
	if ( min < 0 || min > 59 ) return 0;
	if ( sec < 0 || sec > 59 ) return 0;
    
	return 1;
}

/*************************************************************************
* fn: time() 函数封装
**************************************************************************/
int Time()
{
	return time( NULL );
}

int Gettimeofday( struct timeval *tv, struct timezone *tz )
{
	return gettimeofday( tv, tz );
}

       
