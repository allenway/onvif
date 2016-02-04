#ifndef __NETSOCKET_H__
#define __NETSOCKET_H__

#include "const.h"

#define SOCKET_BLOCK	0
#define SOCKET_NOBLOCK	1

#define TCP_CONNECT_TIMEOUT		1 // TCP connect³¬Ê±

int SocketUdpListen( int *pUdpSocket, unsigned short port );
int SocketUdpListenAddr( int *pUdpSocket, char *ip, unsigned short port );
int SocketUdpClient( int *pUdpSocket);
int SocketTcpListen( int *pTcpSocket, unsigned short port, int blockFlag );
int SocketTcpConnect( char *ip, unsigned short port, int blockFlag, 
                     int timeoutMs, int sendBufSize, int *pTcpSocket );
int SocketTcpConnectTimtout( int *pTcpSocket, char *ip, unsigned short port, int timeoutMs  );
int SocketTcpConnectBlock( int *pTcpSocket, char *ip,unsigned short port );
int Accept( int listenSocket );
int AcceptNoblock( int listenSocket );
int SetSocketNonblock( int socket );
int SelectRead( int socket, int maxMsec );
int SelectWrite( int socket, int maxMsec );
int Getsockname( int socket, char *pIp, int *pPort );
int UdpSendto( int socket, char *ip, unsigned short port, 
            	unsigned char *pBuf, unsigned int sendLen );
void Ntop( struct sockaddr_in netAddr, char *addr, ushort *port );
int Connect( int sockfd, char *ip, ushort port );
int Sendn( int socket, const void *buf, uint len );

#ifdef SOCKET_LIB_DEBUG
struct sockaddr_in 
{
	ushort	sin_family;    /* Address family	    */
	ushort	sin_port;    /* Port number	        */
	struct  in_addr	sin_addr;    /* Internet address	    */
  	uchar   sin_zero[8];
};
struct in_addr
{
	uint s_addr;
};
#endif 

#endif  

