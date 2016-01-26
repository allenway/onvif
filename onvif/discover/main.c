#include "soapH.h"
#include "../gsoap/plugin/wsddapi.h"
// 必须包含nsmap文件，否则编译通不过
#include "wsdd.nsmap"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

//采用单播形式发送probe，发送会马上阻塞等待一个事件响应，事件参考event.c
//只能接收一个event，用于指定地址probe，比如手动添加IPC
static int probeUnicast(const char *endpoint, const char * types, const char *scopes,int timeout)
{
	struct soap *soap = soap_new1(SOAP_IO_UDP); // to invoke messages
	int ret;
	const char *id = soap_wsa_rand_uuid(soap);
	//设置超时时间,>0单位为秒 =0 用不超时 <0单位为微秒
	soap->accept_timeout = soap->recv_timeout = soap->send_timeout = timeout;
	ret = soap_wsdd_Probe(soap,
			  SOAP_WSDD_MANAGED,//SOAP_WSDD_ADHOC,	// ad-hoc mode
			  SOAP_WSDD_TO_TS,	// to a TS
			  endpoint, // address of TS; "soap.udp://239.255.255.250:3702"
			  id,	// message ID
			  NULL, // ReplyTo,表示回应的message ID,因为是主动回发起，所以没有，填NULL
			  types, //types,搜寻的设备类型"dn:NetworkVideoTransmitter tds:Device"
			  scopes,    //scopes,指定搜索范围，无填 NULL
			  NULL);   //match by，匹配规则，无填 NULL
	if(ret!=SOAP_OK)
	{
		soap_print_fault(soap, stderr);
		printf("soap_wsdd_Probe error,ret=%d\n",ret);
	}
	soap_end(soap);
	soap_free(soap);
	return ret;
}

//采用多播形式发送probe，立即返回不阻塞等待事件响应，获取事件响应要主动接下对应的sock或调用soap_wsdd_listen函数，事件参考event.c
//可以接收多个event，用于探测网络中存在的IPC
static int probeMulticast(const char *endpoint, const char * types, const char *scopes,int timeout)
{
	struct soap *serv = soap_new1(SOAP_IO_UDP); // to invoke messages
	int ret;
	const char *id = soap_wsa_rand_uuid(serv);
	
	ret = soap_wsdd_Probe(serv,
			  SOAP_WSDD_ADHOC,//SOAP_WSDD_ADHOC,	// ad-hoc mode
			  SOAP_WSDD_TO_TS,	// to a TS
			  endpoint, // address of TS; "soap.udp://239.255.255.250:3702"
			  id,	// message ID
			  NULL, // ReplyTo,表示回应的message ID,因为是主动回发起，所以没有，填NULL
			  types, //types,搜寻的设备类型"dn:NetworkVideoTransmitter tds:Device"
			  scopes,    //scopes,指定搜索范围，无填 NULL
			  NULL);   //match by，匹配规则，无填 NULL

	if(ret!=SOAP_OK)
	{
		soap_print_fault(serv, stderr);
		printf("soap_wsdd_Probe error,ret=%d\n",ret);
		goto ERR0;
	}	
	if (!soap_valid_socket(serv->socket))
	{ 
		soap_print_fault(serv, stderr);
		printf("sock is error\n");
  		ret = -1;
		goto ERR0;
	}
	serv->master = serv->socket;//必须指定，否则无法监听
	ret = soap_wsdd_listen(serv,timeout);
	
	if(ret!=SOAP_OK)
	{
		soap_print_fault(serv, stderr);
		printf("soap_wsdd_listen error,ret=%d\n",ret);
		goto ERR0;
	}
ERR0:
	soap_end(serv);
	soap_free(serv);
	return ret;
}

//监听指定端口，事件参考event.c
//用于监听IPC上线或下线、或响应客户探测
//timeout设置超时时间,>0单位为秒 =0 用不超时 <0单位为微秒
static int listenPort(int port,int timeout)
{
	struct soap *serv = soap_new1(SOAP_IO_UDP); // to invoke messages
	int ret;
	//绑定端口号
	if (!soap_valid_socket(soap_bind(serv,NULL,port, 100)))
	{ 
		soap_print_fault(serv, stderr);
		printf("soap_bind error\n");
  		ret = -1;
		goto ERR0;
	}
	//加入多播地址
	struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(serv->socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
		printf("add multiaddr is error\n");
    	goto ERR0;
    }
	//监听
	ret = soap_wsdd_listen(serv,timeout);
	if(ret!=SOAP_OK)
	{
		soap_print_fault(serv, stderr);
		printf("soap_wsdd_listen error,ret=%d\n",ret);
		goto ERR0;
	}
	printf("soap_wsdd_listen return\n");
ERR0:
	soap_end(serv);
	soap_free(serv);
	return ret;
}
 
int main()
{
	int ret;
	char *endpoint = "soap.udp://239.255.255.250:3702";
	char *types = NULL;
	char *scopes = NULL;
	
	while(1)
	{
		ret = 0;
	//	listenPort(3702,0);
#if 0	//unicast test
		endpoint = "soap.udp://192.168.110.71:3702";
		types = "dn:NetworkVideoTransmitter";
		ret = probeUnicast(endpoint,types,scopes,5);
		types = "tds:Device";
		ret |= probeUnicast(endpoint,types,scopes,5);
#else 	//multicast test
		char *endpoint = "soap.udp://192.168.16.128:3702";
		ret = probeMulticast(endpoint,types,scopes,5);
#endif
		if(ret)
			printf("probe failed\n");
		else
			printf("probe successful\n");
		sleep(5);
	}
	return 0;
} 
