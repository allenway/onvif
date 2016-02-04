/********************************************************************************
**  Copyright (c) 2013, 深圳市动车电气自动化有限公司, All rights reserved.
**  author        :  sven
**  version       :  v1.0
**  date           :  2013.10.10
**  description  : 封装 socket通信 相关函数
********************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include "const.h"
#include "debug.h"
#include "netSocket.h"

#include <netinet/tcp.h>

int SocketUdpListen( int *pUdpSocket, unsigned short port )
{
	int nRet = -1;
    *pUdpSocket = -1;
	int nSocket = socket( AF_INET, SOCK_DGRAM, 0 );
	if ( nSocket == -1 ) return nRet;

	unsigned long ul = 1;
 	ioctl( nSocket, FIONBIO, &ul );  // 设置为非阻塞模式
     
	struct sockaddr_in my_addr = { 0 };
	my_addr.sin_family	    = AF_INET;
	my_addr.sin_port	    = htons( port );
	my_addr.sin_addr.s_addr	= INADDR_ANY;
	nRet = bind( nSocket, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_in) );
    
	if ( nRet == -1 )
    {
    	close( nSocket );
    }
	else
    {
    	int bReUseAddr = 1;
    	nRet = setsockopt( nSocket, SOL_SOCKET, SO_REUSEADDR,
                    (const char *)&bReUseAddr, sizeof(bReUseAddr) );
    	if ( nRet == -1 ) SVPrint( "Setsockopt SO_REUSEADDR Error !\r\n" );
        *pUdpSocket = nSocket;
    }
	return nRet;
}

/****************************************************
* 指定ip及port来listen
******************************************************/
int SocketUdpListenAddr( int *pUdpSocket, char *ip, unsigned short port )
{
	int nRet = -1;
    *pUdpSocket = -1;
	int nSocket = socket( AF_INET, SOCK_DGRAM, 0 );
	if ( nSocket == -1 ) return nRet;

	unsigned long ul = 1;
 	ioctl( nSocket, FIONBIO, &ul );  // 设置为非阻塞模式
     
	struct sockaddr_in my_addr = { 0 };
	my_addr.sin_family	    = AF_INET;
	my_addr.sin_port	    = htons( port );
	my_addr.sin_addr.s_addr	= inet_addr(ip);
	nRet = bind( nSocket, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_in) );
    
	if ( nRet == -1 )
    {
    	close( nSocket );
    }
	else
    {
    	int bReUseAddr = 1;
    	nRet = setsockopt( nSocket, SOL_SOCKET, SO_REUSEADDR,
                    (const char *)&bReUseAddr, sizeof(bReUseAddr) );
    	if ( nRet == -1 ) SVPrint( "Setsockopt SO_REUSEADDR Error !\r\n" );
        *pUdpSocket = nSocket;
    }
	return nRet;
}

int SocketUdpClient( int *pUdpSocket)
{
	int nRet             = -1;
    *pUdpSocket         = -1;
	unsigned long ul     = 1;    
	int bReUseAddr         = 1;
    
	int nSocket = socket( AF_INET, SOCK_DGRAM, 0 );
	if ( nSocket == -1 ) 
    {
    	return nRet;
    }

 	nRet = ioctl( nSocket, FIONBIO, &ul );  // 设置为非阻塞模式     
	if ( nRet == -1 )
    {
    	close( nSocket );
    }
	else
    {
    	nRet = setsockopt( nSocket, SOL_SOCKET, SO_REUSEADDR,
                    (const char *)&bReUseAddr, sizeof(bReUseAddr) );
    	if ( 0 == nRet )
        {
            *pUdpSocket = nSocket;
        }
    }

	if( nRet != 0 )
    {
    	SVPrint( "error:SocketUdpClient:%s!\r\n", STRERROR_ERRNO );
    }
    
	return nRet;
}

int SocketTcpListen( int *pTcpSocket, unsigned short port, int blockFlag )
{
	int nRet = -1;
    *pTcpSocket = -1;
	int nSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( nSocket == -1 ) return nRet;
    
	int bReUseAddr = 1;
	setsockopt( nSocket, SOL_SOCKET, SO_REUSEADDR,
            (const char*)&bReUseAddr, sizeof(bReUseAddr) );
            
	if(SOCKET_NOBLOCK == blockFlag)        
    {
    	unsigned long ul = 1;
     	ioctl( nSocket, FIONBIO, &ul );  //设置为非阻塞模式	
    }
     
	struct sockaddr_in my_addr = { 0 };
	my_addr.sin_family	    = AF_INET;
	my_addr.sin_port	    = htons( port );
	my_addr.sin_addr.s_addr	= INADDR_ANY;
	nRet = bind( nSocket, (struct sockaddr*)&my_addr, sizeof(struct sockaddr_in) );
	if ( nRet == -1 )
    {
    	close( nSocket );
    }
	else
    {
    	nRet = listen( nSocket, 32 );
    	if ( nRet == -1 ) close( nSocket );
    	else *pTcpSocket = nSocket;
    }
	return nRet;
}

/******************************************************************************
* fn: 创建一个TCP连接, connect的超时为 TCP_CONNECT_TIMEOUT
* ip: 服务器的IP
* port: 服务器的listen 端口
* blockFlag: 是否阻塞, SOCKET_BLOCK or SOCKET_NOBLOCK
* timeoutMs: 超时时间ms, 大于1000 的时候有效, 否则使用默认值
* sendBufSize: 发送缓冲区大小, 大于512 时生效, 否则使用默认值
* pTcpSocket: out, 创建成功后的socket
*******************************************************************************/
int SocketTcpConnect( char *ip, unsigned short port, int blockFlag, 
                     int timeoutMs, int sendBufSize, int *pTcpSocket  )
{
	int nRet = -1;    
	int nSocket;
	int bReUseAddr = 1;
	uint ul;
	struct timeval tval;        
	struct sockaddr_in my_addr = { 0 };
    
	if( NULL == pTcpSocket || NULL == ip ) 
    {
    	SVPrint( "error:NULL == pTcpSocket || NULL == ip!\r\n" );
    	return -1;
    }
    
    *pTcpSocket = -1;
	nSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( nSocket == -1 ) return nRet;

	if( 0 == nRet )
    {
    	nRet = setsockopt( nSocket, SOL_SOCKET, SO_REUSEADDR,
            (const char*)&bReUseAddr, sizeof(bReUseAddr) );    
    }
	if( 0 == nRet && timeoutMs >= 1000 )
    {
    	tval.tv_sec     = timeoutMs / 1000;
    	tval.tv_usec     = timeoutMs % 1000;
    	nRet = setsockopt( nSocket, SOL_SOCKET, SO_SNDTIMEO,
                (const char*)&tval, sizeof(tval) );         
    }
	if( 0 == nRet )
    {
    	my_addr.sin_family	    = AF_INET;
    	my_addr.sin_port	    = htons( port );
    	my_addr.sin_addr.s_addr	= inet_addr( ip );

    	nRet = connect(nSocket, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_in));    
        //FiPrint2( "connect to ip(%s) port(%d) nRet = %d, (%s)!\r\n", ip, port, nRet, STRERROR_ERRNO );
    }
	if ( 0 == nRet && SOCKET_NOBLOCK == blockFlag)
    {
    	ul = 1;
     	nRet = ioctl( nSocket, FIONBIO, &ul );  //设置为非阻塞模式	
    }
	if( 0 == nRet && sendBufSize >= 512 )
    {
    	nRet = setsockopt( nSocket, SOL_SOCKET, SO_SNDBUF, 
                    (char *)&sendBufSize, sizeof(sendBufSize) );        
    }
    
	if( 0 == nRet )
    {
        *pTcpSocket = nSocket;
    }    
	else
    {
    	SVPrint( "error:%s:%s!\r\n", __FUNCTION__, STRERROR_ERRNO );
    }    
    
	return nRet;
}

/*******************************************************************************
* fn: 创建一个TCP非阻塞的连接, connect的超时为 timeoutMs
* pTcpSocket: out, 创建成功后的socket
* ip: 服务器的IP
* port: 服务器的listen 端口
* timeoutMs: 连接的超时时间,ms
********************************************************************************/
int SocketTcpConnectTimtout( int *pTcpSocket, char *ip, unsigned short port, int timeoutMs  )
{
	int nRet = -1;    
	int nSocket;
	int bReUseAddr = 1;
	struct timeval tval;

	tval.tv_sec     = timeoutMs / 1000;
	tval.tv_usec     = timeoutMs % 1000;
    
	if( NULL == pTcpSocket || NULL == ip ) return -1;
    
    *pTcpSocket = -1;
	nSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( nSocket == -1 ) return nRet;
    
	nRet = setsockopt( nSocket, SOL_SOCKET, SO_REUSEADDR,
            (const char*)&bReUseAddr, sizeof(bReUseAddr) );    
	if( nRet == -1 ) return nRet;
    
	nRet = setsockopt( nSocket, SOL_SOCKET, SO_SNDTIMEO,
            (const char*)&tval, sizeof(tval) );         
    
	struct sockaddr_in my_addr = { 0 };
	my_addr.sin_family	    = AF_INET;
	my_addr.sin_port	    = htons( port );
	my_addr.sin_addr.s_addr	= inet_addr( ip );

	nRet = connect(nSocket, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_in));    
    //FiPrint2( "connect to ip(%s) port(%d) nRet = %d, (%s)!\r\n", ip, port, nRet, STRERROR_ERRNO );
	if ( nRet == -1 )
    {
    	close( nSocket );
    }
	else
    {

    	unsigned long ul = 1;
    	nRet = ioctl( nSocket, FIONBIO, &ul );  //设置为非阻塞模式	
    	if( 0 != nRet )
        {
        	close( nSocket );
        	return nRet;
        }

     	int sendBufSize = 128*1024;
     	if( nRet == 0)
         {
        	nRet = setsockopt( nSocket, SOL_SOCKET, SO_SNDBUF, (char *)&sendBufSize, sizeof(sendBufSize) );
        	if( 0 != nRet )
            {
            	close( nSocket );
            	return nRet;
            }
        }
        *pTcpSocket = nSocket;
    }
    
	return nRet;
}


/******************************************************************************
* fn: 创建一个tcp连接,功能和InitTcpConnect 类似,但有区别
      不同点在于它是没有设置是否阻塞的参数,并且connect 是无限期地等待
********************************************************************************/
int SocketTcpConnectBlock( int *pTcpSocket, char *ip, unsigned short port )
{
	int nRet = -1;
	if(NULL == pTcpSocket || NULL == ip) return -1;
    
    *pTcpSocket = -1;
	int nSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( nSocket == -1 ) return nRet;
    
	int bReUseAddr = 1;
	setsockopt( nSocket, SOL_SOCKET, SO_REUSEADDR,
            (const char*)&bReUseAddr, sizeof(bReUseAddr) );    
     
	struct sockaddr_in my_addr = { 0 };
	my_addr.sin_family	    = AF_INET;
	my_addr.sin_port	    = htons( port );
	my_addr.sin_addr.s_addr	= inet_addr( ip );

	nRet = connect(nSocket, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_in));    
	if ( nRet == -1 )
    {
    	close( nSocket );
    }
	else
    {
     	int sendBufSize = 128*1024;
     	if( nRet == 0)
        	nRet = setsockopt( nSocket, SOL_SOCKET, SO_SNDBUF, (char *)&sendBufSize, sizeof(sendBufSize) );
    
        *pTcpSocket = nSocket;
    }
    
	return nRet;
}

/******************************************************************************
* fn: accept
* 返回: -1, 出错; 否则返回一个accept 后的socket
******************************************************************************/
int Accept( int listenSocket )
{
	struct sockaddr_in clientAddr;
	int acceptSocket;
	int len = sizeof( clientAddr );    

	memset( &clientAddr, 0x00, sizeof(clientAddr) );
	acceptSocket = accept( listenSocket,(struct sockaddr *)&clientAddr, (socklen_t *)&len );
    
	return acceptSocket;
}


/*****************************************************************************
* fn: accept,并且把accept 后的socket 设置成非阻塞
* 返回: -1, 出错; 否则返回一个accept 后的socket
******************************************************************************/
int AcceptNoblock( int listenSocket )
{
	struct sockaddr_in clientAddr;
	int acceptSocket, ret;
	int len = sizeof( clientAddr );    

	memset( &clientAddr, 0x00, sizeof(clientAddr) );
	acceptSocket = accept( listenSocket,(struct sockaddr *)&clientAddr, (socklen_t *)&len );
//	SVPrint( "acceptSocket(%d) = accept()!\r\n", acceptSocket );
	if( -1 != acceptSocket )
    {
    	ret = SetSocketNonblock( acceptSocket );
        //////////////////////////////////
        //long o = 1; 
        //socklen_t ol = sizeof(long);
        //setsockopt(acceptSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&o, ol);
        //////////////////////////////////
        
//    	SVPrint( "ret(%d) = SetSocketNonblock!\r\n", ret );
    	if( -1 == ret )
        {
        	close( acceptSocket );
        	acceptSocket = -1;
            ERRORPRINT("SetSocketNonblock error return:%d",ret);
        }
    }

	return acceptSocket;
}

/***************************************************************************
* fn: 把socket 设置成非阻塞
****************************************************************************/
int SetSocketNonblock( int socket )
{
	int opts;

	opts = fcntl( socket, F_GETFL );
	if( opts < 0 )
    {
    	SVPrint( "fcntl error(%s)!\r\n", STRERROR_ERRNO );
    	return -1;
    }

	opts = opts|O_NONBLOCK;
	if( fcntl( socket, F_SETFL, opts)<0 )
    {
    	SVPrint( "fcntl error(%s)!\r\n", STRERROR_ERRNO );
    	return -1;
    } 

	return 0;
}

/**************************************************************************
* fn: select socket 是否可读
* socket: 检测的socket
* maxMsec: 超时时间,单位毫秒
***************************************************************************/
int SelectRead( int socket, int maxMsec )
{
	int nRet = -1;
	fd_set fd; 
	FD_ZERO( &fd );
  	FD_SET( socket, &fd );

	struct timeval timeout;
	timeout.tv_sec	= maxMsec / 1000;
  	timeout.tv_usec	= (maxMsec % 1000) * 1000;
    
  	nRet = select( socket + 1, &fd, NULL, NULL, &timeout );
	if ( nRet > 0 )
    {
      	if ( FD_ISSET(socket, &fd)) nRet = 1;
      	else nRet = -1;
    }
  	return nRet;
}

/**************************************************************************
* fn: select socket 是否可写
* socket: 检测的socket
* maxMsec: 超时时间,单位毫秒
***************************************************************************/
int SelectWrite( int socket, int maxMsec )
{
	int nRet = -1;
	fd_set fd; 
	FD_ZERO( &fd );
  	FD_SET( socket, &fd );

	struct timeval timeout;
	timeout.tv_sec	= maxMsec / 1000;
  	timeout.tv_usec	= (maxMsec % 1000) * 1000;
    
  	nRet = select( socket + 1, NULL, &fd, NULL, &timeout );
	if ( nRet > 0 )
      {
      	if ( FD_ISSET(socket, &fd) > 0 ) nRet = 1;
      	else nRet = -1;
      }
  	return nRet;
}

/**************************************************************************
* fn: accept
* 返回: -1, 出错; 否则返回一个accept 后的socket
**************************************************************************/
int Getsockname( int socket, char *pIp, int *pPort )
{
	int ret;
	struct sockaddr_in sai = { 0 };
	uint slen = sizeof( sai );
	char addr[NET_ADDRSIZE] = { 0 };
	const char *ptr;

	ret = getsockname( socket, (struct sockaddr *)&sai, &slen );
	if( 0 == ret )
    {
    	if( NULL != pPort )
        {
            *pPort = ntohs(sai.sin_port);
        }
    	if( NULL != pIp )
        {
        	ptr = inet_ntop( AF_INET, &sai.sin_addr, addr, sizeof(addr) );
        	if( NULL != ptr )
            {
            	strcpy( pIp, addr );
            }
        }
    }

	return ret;
}

static int Sendto( int fd, const void *ptr, int nbytes, int flags,
                   	const struct sockaddr *sa, socklen_t salen )
{
	int ret = 0;
	if ( nbytes != sendto(fd, ptr, nbytes, flags, sa, salen) )
    {
    	ret = -1;
    }

	return ret;
}

/***************************************************************************
* fn: udp 发送
* socket: SocketUdpClient() 得到的值
* ip: 发送到这个ip 地址
* port: 目的端口
* pBuf: 发送内容buf
* sendLen: 发送长度
*****************************************************************************/
int UdpSendto( int socket, char *ip, unsigned short port, 
            	unsigned char *pBuf, unsigned int sendLen )
{    
	int ret;
	struct sockaddr_in	srvaddr;    
	int socketLen = sizeof( srvaddr );
    
	srvaddr.sin_addr.s_addr = inet_addr( ip );
	srvaddr.sin_port         = htons( port );    
	srvaddr.sin_family	    = AF_INET; 

	ret = Sendto( socket, pBuf, sendLen, 0, (struct sockaddr *)&srvaddr, socketLen );

	return ret;
}

/********************************************************************
* fn: 网络地址翻译
* netAddr: 网络地址
* addr: out, 得到的ip 地址
* port: out, 得到的端口
***********************************************************************/
void Ntop( struct sockaddr_in netAddr, char *addr, ushort *port )
{
	if(NULL != port )
    {
        *port     = ntohs( netAddr.sin_port );        
    }

	if(NULL != addr )
    {
    	inet_ntop( AF_INET, &netAddr.sin_addr, addr, 20 );
    }
}

int Connect( int sockfd, char *ip, ushort port )
{
	int ret;
	struct sockaddr_in remoteAddr = { 0 };

	remoteAddr.sin_family	    = AF_INET;
	remoteAddr.sin_port	        = htons( port );
	remoteAddr.sin_addr.s_addr	= inet_addr( ip );
    
	ret = connect( sockfd, (struct sockaddr *)&remoteAddr, sizeof(remoteAddr) );

	return ret;
}

/******************************************************************************
* fn: 往socket 发送len 个字节,直到发送完毕或者出错才返回
* 返回值: 0, 标识成功, -1, 标识失败
* 特别注意: 这个sendn 的封装返回值根一般的send 的返回值不一样,
********************************************************************************/
int Sendn( int socket, const void *buf, uint len )
{
	int ret = -1;
	uint leftLen = len;
	uint off = 0, count = 0;
    
	while( leftLen > 0 )
    {
    	ret = SelectWrite( socket, 1000 );
    	if( ret > 0 )
        {
        	count = 0;
        	ret = write( socket, ((char *)buf) + off, leftLen );
            //ret = send( socket, ((char *)buf) + off, leftLen ,0);
        	if( ret <= 0 )
            {
            	ERRORPRINT( "ret(%d) = write( len = %u) error:%s!\r\n", 
                            	ret, leftLen, STRERROR_ERRNO );
            	ret = -1;
            	break; // while( leftLen > 0
            }
            
        	leftLen -= ret;
        	off += ret;
            //CORRECTPRINT("has sended :%d,left:%d\n",off,leftLen);
        }
    	else if( 0 == ret )
        {
        	if( ++count >= 60 ) //如果半分钟都没有反应,则退出
            {
            	ret = -1;
            	break; // while( leftLen > 0
            }
        }
    	else
        {
        	ERRORPRINT( "ret(%d) = SelectWrite() error:%s!\r\n", ret, STRERROR_ERRNO );
        	break;
        }
    }

	if( ret != -1 )
    {
    	ret = 0;
    }

	return ret;
}

