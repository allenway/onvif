/********************************************************************************
**  Copyright (c) 2013, 深圳市动车电气自动化有限公司, All rights reserved.
**  author        :  sven
**  version       :  v1.0
**  date           :  2013.10.10
**  description  : 封装 linux 文件读写 相关函数
********************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/mount.h>
#include "timeExchange.h"
#include "malloc.h"
#include "linuxFile.h"

/******************************************************************************
* flags: O_RDONLY|O_WRONLY|O_RDWR|O_CREAT|O_TRUNC
*******************************************************************************/
int Open( const char *pathname, int flags )
{
	return open( pathname, flags );
}

int Write( int fd, const void *buf, unsigned int count )
{
	return write( fd, buf, count );
}

/*******************************************************************************
* fn: 安全写函数封装
* fd: 操作句柄
* buf: 被写的内容
* size: 要写多少个字节
* 返回: < 0, 出错;大于0,写入的字节数
*******************************************************************************/
int Writen( int fd, void *buf, unsigned int size )
{
	unsigned int nleft;
	int nwritten;
	char *ptr;

	ptr     = (char *)buf;
	nleft     = size;
	while (nleft > 0) 
    {
    	nwritten = write( fd, ptr, nleft );
    	if( nwritten < 0 ) 
        {
        	if( EINTR == errno )
            {
            	nwritten = 0; /* and call write() again */
            }
        	else
            {
            	return -1; /* error */
            }
        }
    	else if( 0 == nwritten )
        {
        	break;
        }
    	else
        {
        	nleft     -= nwritten;
        	ptr     += nwritten;
        	usleep( 10 );
        }
    }
    
	return size - nleft;
}

int Read( int fd, void *buf, unsigned int count )
{
	return read( fd, buf, count );
}

int Readn( int fd, void *buf, unsigned int size )
{
	unsigned int nleft;
	int nread;
	char *ptr;

	ptr     = (char *)buf;
	nleft     = size;
	while( nleft > 0 ) 
    {
    	nread = read( fd, ptr, nleft );
    	if( nread < 0 ) 
        {
        	if( errno == EINTR )
            {
            	nread = 0; /* and call read() again */
            }
        	else
            {
            	return -1;
            }
        } 
    	else if( 0 == nread )
        {
        	break;
        }
    	else
        {
        	nleft     -= nread;
        	ptr     += nread;            
        	NanosleepZero();        
        }
    }
    
	return( size - nleft ); 
}

void Close( int fd )
{
	close( fd );
}

/*******************************************************************************
* sven explain: 封装strsep,并去掉分隔符之间为'\0'的情况。
                 函数执行完后,stringp指向的地址不变,但它里面的分隔符
                 delim已经被 "\0"代替了
  stringp     : 指向某个字符串的指针的地址,例如 char *ptr, 则 stringp = &ptr
  delim	    : 以该字符串为分隔符分隔
  返回	    : 分割后的字符串的指针
********************************************************************************/
char *Strsep( char **stringp, const char *delim )
{
	char *p;

	while (1)
    {
    	p = strsep( stringp, delim );
    	if (p == NULL)
        {
        	return NULL;
        }
    	else
        {
        	if (p[0] == '\0')
            	continue;
        	else
            	break;
        }
    }
	return p;
}

char *Strstr( char *haystack, char *needle )
{
	return strstr( haystack, needle );
}

int Strlen( const char *pstr )
{
	if( NULL != pstr )
    {
    	return strlen( pstr );
    }

	return 0;
}

int Readline( int fd, char *vptr, int maxlen )
{
	int n, rc;
	char c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++)
    {
	again:
    	if ((rc = read(fd, &c, 1)) == 1)
        {
            *ptr++ = c;
        	if (c == '\n')
            	break; /* newline is stored, like fgets() */
        }
    	else if (rc == 0)
        {
            *ptr = 0;
        	return(n - 1); /* EOF, n-1 bytes were read */
        }
    	else
        {
        	if (errno == EINTR)
            	goto again;
        	return(-1);
        }
    }
    *ptr = 0; /* null terminate like fgets() */
	return(n);
}

int Unlink( const char *pathname )
{
	return unlink( pathname );
}

#if !defined(S_ISDIR)
#define S_ISDIR(m) (((m)&S_IFMT) == S_IFDIR)
#endif

/*******************************************************************************
* fn: 实现mkdir -p PATH 功能
* newPath: 等价于mkdir() 的第一个参数
* mode: 等价于mkdir() 的第二个参数
*****************************************************************************/
int MakePath( char *newPath, int mode )
{
 char     *savedPath, *cp;
 int 	saved_ch;
 struct stat st;
 int 	rc;

 cp = savedPath = strdup(newPath);
 while (*cp && *cp == '/') 
 {
     ++cp;
 }
 while (1) 
 {
     while(*cp && *cp != '/')
     {
         ++cp;
     }
     if( (saved_ch = *cp) != 0 )
     {
         *cp = 0;
     } 
     if( (rc = stat(savedPath, &st)) >= 0 ) 
     {
         if( !S_ISDIR(st.st_mode) ) 
         {
             errno = ENOTDIR;
             rc = -1;
             break;
         }
     } 
     else 
     {
         if( errno != ENOENT ) 
         {
            break;
         }
         
         if( (rc = mkdir(savedPath, mode)) < 0 ) 
         {
             if( errno != EEXIST )
             {
                 break;
             }
             if( (rc = stat(savedPath, &st)) < 0 )
             {
                 break;
             }
             if( !S_ISDIR(st.st_mode) ) 
             {
                 errno = ENOTDIR;
                 rc = -1;
                 break;
             }                
         }
     }
     
     if (saved_ch != 0)
     {
         *cp = saved_ch;
     }
     while (*cp && *cp == '/') 
     {
         ++cp;
     }
     if (*cp == 0)
     {
         break;
     }
 }
 
 Free( savedPath );
 
 return rc;
 
}

/***************************************************************************
* fn: 文件权限检测
* pathname: 文件全名,包括路径
* mode: R_OK,检测读权限; W_OK,检测写权限; X_OK,检测执行权限; F_OK,检测是否存在 
********************************************************************************/
int Access( char *pathname, int mode )
{
	return access( (const char *)pathname, mode );
}

int Unlink( char *pathname )
{    
	return unlink( (const char *)pathname );
}

/*******************************************************************************
* fn: 标准的文件打开函数
* pathname: 文件名
* mode: 
	r, 以只读方式打开,打开后光标在文件开头.
	r+, 以读写方式打开,打开后光标在文件开头.
	w, 以写方式打开,打开后光标在文件开头.如果文件存在,则删除所有内容;如果文件不存在则创建.
	w+, 以读写方式打开,打开后光标在文件开头.如果文件存在,则删除所有内容;如果文件不存在则创建
	a, 以写方式打开,打开后光标在文件结尾.如果文件不存在则创建.
	a+, 以读写方式打开,打开后光标在文件结尾.如果文件不存在则创建.
******************************************************************************/
FILE *Fopen( char *pathname, char *mode )
{
	return fopen( (const char *)pathname, (const char *)mode );
}

int Fclose( FILE *fp )
{
	return fclose( fp );
}

/*****************************************************************************
* fn: 读取
* ptr: out, 存放读取到的值
* size: 读多少次
* nmemb: 每次读多少个字节
* stream: Fopen() 的返回
* 返回: 读取到的字节数
*******************************************************************************/
int Fread( void *ptr, int size, int nmemb, FILE *stream )
{
	return fread( ptr, size, nmemb, stream );
}

/******************************************************************************
* fn: 写
* ptr: 存放被写的内容
* size: 写多少次
* nmemb: 每次写多少个字节
* stream: Fopen() 的返回
* 返回: 写入的字节数
********************************************************************************/
int Fwrite( void *ptr, int size, int nmemb, FILE *stream )
{
	return fwrite( (const void *)ptr, size, nmemb, stream );
}

/*****************************************************************************
* fn: 写入字符串
* s: 存放被写的内容
* stream: Fopen() 的返回
* 返回: 写入的字节数
*********************************************************************************/
int Fputs( char *s, FILE *stream )
{
	return fputs( (const char *)s, stream );
}

void *Memmove( void *dest, const void *src, int n )
{
	return memmove( dest, src, n );
}

void *Memmem( const void *haystack, int haystacklen,
                const void *needle, int needlelen )
{
	return memmem( haystack,  haystacklen,
                    needle, needlelen );
}

int Strcmp(const char *s1, const char *s2)
{
	return strcmp( s1, s2 );
}

int Strncmp(const char *s1, const char *s2, int n)
{
	return strncmp( s1, s2, n);
}
char *Strcat(char *s1, const char *s2)
{
	return strcat( s1, s2 );
}

void *Memset(void *s, int c, int n)
{
	return memset( s, c, n);
}

char *Strncpy( char *dest, const char *src, int n )
{
	return strncpy( dest, src, n );
}

char *Strcpy( char *dest, const char *src)
{
	return strcpy( dest, src );
}

void *Memcpy( void *dest, const void *src, int n )
{
	return memcpy( dest, src, n );
}

int Usleep( uint usec )
{
	return usleep( usec );
}

int Ssleep( uint sec )
{
	return sleep( sec );
}

int Statfs( const char *path, struct statfs *buf )
{
	return statfs( path, buf );
}

DIR *Opendir( const char *name )
{
	return opendir( name );
}

int Lstat( const char *path, struct stat *buf )
{
	return lstat( path, buf );
}

struct dirent *Readdir( DIR *dir )
{
	return readdir( dir );
}                   

int Closedir( DIR *dir )
{
	return closedir( dir );
}

int Chmod( const char *path, unsigned int mode )
{
	return chmod( path, (mode_t)mode );
}

int Rmdir( const char *pathname )
{
	return rmdir( pathname );
}

int Mkdir( const char *pathname, uint mode )
{
	return mkdir( pathname, (mode_t)mode );
}

int Mount( const char *source, const char *target,
                 const char *filesystemtype, unsigned long mountflags,
                 const void *data )
{
	return mount(source, target, filesystemtype, mountflags, data );
}                     

int Umount( const char *target )
{
	return umount( target );
}    

int Strcasecmp( const char *s1, const char *s2 )
{
	return strcasecmp( s1, s2 );
}

/****************************************************************************
* fn: 向文件中写入所有字节才返回
* 返回: -1, 失败,需要写入的字节数
****************************************************************************/
int writenFileComplete( int fd, const void *vptr, int n )
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = (char *)vptr;
	nleft = n;
	while (nleft > 0) 
    {
    	if ( (nwritten = write(fd, ptr, nleft)) <= 0) 
        {
        	if (errno == EINTR)
            	nwritten = 0; /* and call write() again */
        	else
            	return(-1); /* error */
        }
    	if (errno == 5)
        	return(-1);
    	nleft -= nwritten;
    	ptr += nwritten;
    }
    
	return(n);
}

int Fsync(int fd)
{
	return fsync( fd );
}

