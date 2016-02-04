#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include "debug.h"
#include "netSocket.h"
#include "thread.h"

static void *ipcThreadHandle(void *)
{
	//连接到IPC,192.168.101.76:10100
	int ipcSocket;
	int ret;
	int port = 10100;
	char *ip = "192.168.101.76";
	
	ret = SocketTcpConnectTimtout(&ipcSocket,ip,port,5000);
	if(0!=ret)
	{
		ERRORPRINT("can't connect ipc %s:%d\n",ip,port);
	}
	CORRECTPRINT("connect ipc %s:%d\n",ip,port);
	SetSocketNonblock(ipcSocket);
	int loop = 1;
	unsigned char buf[1024];
	int bufsize = 1024;
	while(loop)
	{

		CORRECTPRINT("ipcThreadHandle,pid=%ld\n",ThreadSelf());
		printf("ipcThreadHandle,pid=%ld\n",ThreadSelf());

		#if 1
		ret = SelectRead(ipcSocket, 2000);
		if(ret==-1)
		{
			ERRORPRINT("SelectRead ipc faild\n");
			break;
		}
		else if(ret==0)
		{
			CORRECTPRINT("SelectRead ipc timeout\n");
			continue;
		}
		#endif
		ret = read(ipcSocket,buf,bufsize);
		if(ret>0)
		{
			PrintHex("read from data ipc:\n",buf,ret,"\ntotal %d bytes\n",ret);
			//查询0x58,ipc 状态回复内容
			if(ret>=21&&buf[0]==0xff&&buf[1]==0x58)
			{
				if(buf[2]==0&&buf[3]==16)
				{
					CORRECTPRINT("====ipc stat====\n");
					CORRECTPRINT("car number:%d\n",buf[4]);
					CORRECTPRINT("locate number:%d\n",buf[5]);
					CORRECTPRINT("online stat:%d\n",buf[6]);
					CORRECTPRINT("type:%d\n",buf[7]);
					CORRECTPRINT("company:%c%c%c%c%c%c%c%c%c%c\n",
					buf[8],buf[9],buf[10],buf[11],buf[12],buf[13],buf[14],
												buf[15],buf[16],buf[17]);
					CORRECTPRINT("version:%d\n",(buf[18]<<8)|buf[19]);
					CORRECTPRINT("===============\n");
					
				}
			}
		}
		else if(ret==0)
		{
			ERRORPRINT("ipc socket is closed\n");
			close(ipcSocket);
		}
		else
		{
			ERRORPRINT("read ipc failed,ret=%d\n",ret);
		}
		int i;
		//回ipc心跳包
		buf[0] = 0xff;
		buf[1] = 0x51;
		buf[2] = 0x00;
		buf[3] = 0x00;
		buf[4] = 0x0;
		for(i=0;i<4;i++)
			buf[4] ^= buf[i];
		ret = SelectWrite(ipcSocket, 2000);
		if(ret==-1)
		{
			ERRORPRINT("SelectWrite ipc faild\n");
			break;
		}
		else if(ret==0)
		{
			CORRECTPRINT("SelectWrite ipc timeout\n");
			continue;
		}
		ret = write(ipcSocket,buf,5);
		if(ret>0)
		{
			CORRECTPRINT("write data to ipc successful\n");
		}
		//查询ipc状态
		buf[0] = 0xff;
		buf[1] = 0x8;
		buf[2] = 0x00;
		buf[3] = 0x00;
		buf[4] = 0x0;
		for(i=0;i<4;i++)
			buf[4] ^= buf[i];
		ret = SelectWrite(ipcSocket, 2000);
		if(ret==-1)
		{
			ERRORPRINT("SelectWrite ipc faild\n");
			break;
		}
		else if(ret==0)
		{
			CORRECTPRINT("SelectWrite ipc timeout\n");
			continue;
		}
		ret = write(ipcSocket,buf,5);
		if(ret>0)
		{
			CORRECTPRINT("write data to ipc successful\n");
		}
		
	}
	return NULL;
}

static void *monitorThreadHandle(void *)
{
	//连接到IPC,192.168.101.76:10100
	int serv;
	int ret;
	int port = 10100;

	//ret = SocketTcpListen(&serv,port,SOCKET_NOBLOCK);
	ret = SocketTcpListen(&serv,port,0);
	if(0!=ret)
	{
		ERRORPRINT("can't SocketTcpListen port:%d\n",port);
	}
	else
		CORRECTPRINT("SocketTcpListen successful port:%d\n",port);
	int loop = 1;
	unsigned char buf[1024];
	int bufsize = 1024;
	while(loop)
	{
		int client;
		char clientIP[64];
		int clientPort;
		//client = Accept(serv);
		struct sockaddr_in clientAddr;
		int len = sizeof( clientAddr );    

		memset( &clientAddr, 0x00, sizeof(clientAddr) );
		client = accept(serv,(struct sockaddr *)&clientAddr, (socklen_t *)&len );
		
		if(client < 0)
		{
			ERRORPRINT("Accept failed\n");
		}
		else
		{
		
			clientPort = ntohs(clientAddr.sin_port);
			inet_ntop( AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
			CORRECTPRINT("client income ip:%s,%d \n",clientIP,clientPort);
		}
		while(1)
		{
			ret = SelectRead(client, 2000);
			if(ret==-1)
			{
				ERRORPRINT("SelectRead client faild\n");
				break;
			}
			else if(ret==0)
			{
				CORRECTPRINT("SelectRead client timeout\n");
				//continue;
			}
			ret = read(client,buf,bufsize);
			if(ret>0)
			{
				PrintHex("read from data client:\n",buf,ret,"\ntotal %d bytes\n",ret);

			}
			else if(ret==0)
			{
				ERRORPRINT("client socket is closed\n");
				close(client);
				break;
			}
			else
			{
				ERRORPRINT("read client failed,ret=%d\n",ret);
			}
			int i;		
			//心跳包
			buf[0] = 0xff;
			buf[1] = 0x1;
			buf[2] = 0x00;
			buf[3] = 0x00;
			buf[4] = 0x0;
			for(i=0;i<4;i++)
				buf[4] ^= buf[i];
			ret = SelectWrite(client, 2000);
			if(ret==-1)
			{
				ERRORPRINT("SelectWrite client faild\n");
				break;
			}
			else if(ret==0)
			{
				CORRECTPRINT("SelectWrite client timeout\n");
				continue;
			}
			ret = write(client,buf,5);
			if(ret>0)
			{
				CORRECTPRINT("write data to client successful\n");
			}
			//回复状态
			buf[0] = 0xff;
			buf[1] = 0x57;
			buf[2] = 0x00;buf[3] = 18;//数据长度
			buf[4] = 1; //车厢号
			buf[5] = 1; //设备类型
			buf[6] = 'r'; 
			buf[7] = 'h';
			buf[8] = 'a';
			buf[9] = '-';
			buf[10] = 'n';
			buf[11] = 'v';
			buf[12] = 'r';
			buf[13] = '\0';
			buf[14] = '\0';
			buf[15] = '\0'; 			//
			buf[16] = 0;buf[17] = 0; //version
			buf[18] = 0;buf[19] = 250;
			buf[20] = 0;buf[21] = 5;
			buf[22] = 0;
			for(i=0;i<22;i++)
				buf[22] ^= buf[i];
			ret = SelectWrite(client, 2000);
			if(ret==-1)
			{
				ERRORPRINT("SelectWrite client faild\n");
				break;
			}
			else if(ret==0)
			{
				CORRECTPRINT("SelectWrite client timeout\n");
				continue;
			}
			ret = write(client,buf,23);
			if(ret>0)
			{
				CORRECTPRINT("write data to client successful\n");
			}
		}
		
	}
	return NULL;
}

int main()
{
	

	int ret;
	#if 1 //连接IPC
	pthread_t ipcThread;
	ret = ThreadCreateCommonPriority(&ipcThread,ipcThreadHandle,NULL);
	if(0!=ret)
	{
		ERRORPRINT("ThreadCreate ipcThread failed\n");
	}
	else
	{
		CORRECTPRINT("ThreadCreate ipcThread successful\n");
	}
	#endif
	#if 1	//连接授权终端
	pthread_t monitorThread;
	ret = ThreadCreateCommonPriority(&monitorThread,monitorThreadHandle,NULL);
	if(0!=ret)
	{
		ERRORPRINT("ThreadCreate monitorThread failed\n");
	}
	else
	{
		CORRECTPRINT("ThreadCreate monitorThread successful\n");
	}
	#endif 
	while(1)
	{
		sleep(5);
	}
    return 0;
}
