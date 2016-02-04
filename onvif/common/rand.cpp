/********************************************************************************
**  Copyright (c) 2013, 深圳市动车电气自动化有限公司, All rights reserved.
**  author        :  sven
**  version       :  v1.0
**  date           :  2013.10.10
**  description  : 产生随机数
********************************************************************************/

#include <stdlib.h>
#include <sys/time.h>

/***********************************************
* fn: 返回一个随机数
************************************************/
int RandInt()
{
	struct timeval tv;
	gettimeofday( &tv, NULL );
	unsigned int seed = tv.tv_sec ^ tv.tv_usec;
	return rand_r( &seed );
}

/****************************************************
* fn: 返回一个随机数n, a <= n <= b;
*****************************************************/
int RandAb( int a, int b )
{
	struct  timeval tv;

	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);
    
	return( a + (int)((double)(b-a+1)*rand()/(RAND_MAX*1.0)));
}

