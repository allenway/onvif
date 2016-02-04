/********************************************************************************
**  Copyright (c) 2013, 深圳市动车电气自动化有限公司, All rights reserved.
**  author        :  sven
**  version       :  v1.0
**  date           :  2013.10.10
**  description  : 对malloc 族函数封装; 提供指针复制的接口
********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "mutex.h"
#include "malloc.h"

/* ===== 对malloc 族函数封装部分 ===== */
void *MeAlloc( long nbytes, const char *file, int line )
{    
	void *ptr = ( void * )malloc( nbytes );
	if ( ptr == NULL )
    	Print( "Malloc Memory Failed, file: %s, line: %d !\r\n", file, line );
        
	return ptr;
}

void *MeCalloc( long count, long nbytes, const char *file, int line )
{    
	void *ptr = ( void * )calloc( count, nbytes );
	if ( ptr == NULL )
    	Print( "Calloc Memory Failed, file: %s, line: %d !\r\n", file, line );
        
	return ptr;
}

void MeFree( void *ptr, const char *file, int line )
{
	if ( ptr != NULL ) free( ptr );
}

void *MeRealloc( void *ptr, long nbytes, const char *file, int line )
{    
	ptr = ( void * )realloc( ptr, nbytes );
	if ( ptr == NULL )
    	Print( "Realloc Memory Failed, file: %s, line: %d !\r\n", file, line );
        
	return ptr;
}
/* ===== end 对malloc 族函数封装部分 ===== */

/* ===== 用于指针复制的部分 ===== */
#define		SHARE_MEMORY_FLAG		0xCCBB2828
static ClMutexLock	s_ShareMemMutex;

void *ShareMeAlloc( long nbytes, const char *file, int line )
{    
	long size = nbytes + sizeof(long)*2;
	long *ptr = ( long * )malloc( size );
	if ( ptr == NULL )
    {
    	Print( "Malloc Share Memory Failed, file: %s, line: %d !\r\n", file, line );
    }
	else
    {
        *ptr = SHARE_MEMORY_FLAG;
        *(ptr + 1) = 1;
    }        
    
	return (void *)(ptr + 2);
}

void *ShareMeCalloc( long count, long nbytes, const char *file, int line )
{    
	long size = nbytes + sizeof(long)*2;
	long *ptr = ( long * )calloc( count, size );
	if ( ptr == NULL )
    {
    	Print( "Calloc Share Memory Failed, file: %s, line: %d !\r\n", file, line );
    }
	else
    {
        *ptr = SHARE_MEMORY_FLAG;
        *(ptr + 1) = 1;
    }        
	return (void *)(ptr + 2);
}

void ShareMeFree( void *ptr, const char *file, int line )
{
	if ( ptr != NULL )
    {
    	long *pmem = (long *)ptr - 2;
    	if ( *pmem == (long)SHARE_MEMORY_FLAG )
        {
        	s_ShareMemMutex.Lock();
            *(pmem + 1) -= 1;
        	if ( *(pmem + 1) == 0 ) 
            {                
            	free( pmem );
            }
        	s_ShareMemMutex.Unlock();
        }
    	else
        {
        	ERRORPRINT( "Free Share Memory Failed, file: %s, line: %d !\r\n", file, line );
            
        }
    }    
}

void *ShareMeCopy( void *ptr, const char *file, int line )
{
	if ( ptr == NULL )
    {
#if 0
    	Print( "Copy Share Memory Failed, Can't Copy NULL ptr; "
                "file: %s, line: %d !\r\n", file, line );
#endif
    	return NULL;
    }
        
	long *pmem = (long *)ptr - 2;
	if ( *pmem == (long)SHARE_MEMORY_FLAG )
    {
    	s_ShareMemMutex.Lock();
        *(pmem + 1) += 1;
    	s_ShareMemMutex.Unlock();
    	return ptr;
    }
	else
    {
    	ERRORPRINT( "Copy Share Memory Failed, Copy Error ptr; "
                "file: %s, line: %d !\r\n", file, line );
        
    }
	return NULL;
}
/* ===== end 用于指针复制的部分 ===== */

/****************************************************************************
* 根据内存的类型来调用free
* ptr: 被free 的指针
* bShareMem: PDATA_FROM_E
*****************************************************************************/
void FreeMs( void *ptr, int bShareMem )
{
	if( PDATA_FROM_SHAREMALLOC == bShareMem )
    {
    	ShareFree( ptr );
    }
	else
    {
    	Free( ptr );
    }
}

