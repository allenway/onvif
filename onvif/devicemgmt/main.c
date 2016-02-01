#include "soapH.h"
// 必须包含nsmap文件，否则编译通不过
#include "DeviceBinding.nsmap"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

int main()
{
	int ret;
	struct _tds__GetServices services;
	struct _tds__GetServicesResponse servicesResponse;
	struct soap *soap = soap_new();
	//get services
	services.IncludeCapability = xsd__boolean__true_;
	ret = soap_call___tds__GetServices(soap,
			"http://192.168.110.71/onvif/device_service",
			NULL,	//soap action
			&services,
			&servicesResponse);
			
	if(ret!=SOAP_OK)
	{
		soap_print_fault(soap,stderr);
		printf("=======GetServices failed\n");
		goto ERR0;
	}
	else
	{
		int i;
		int services_num;
		struct tds__Service *s;;
		services_num = servicesResponse.__sizeService;
		printf("=======GetServices successful\n");
		printf("services num:%d\n",services_num);

		s = servicesResponse.Service;
		for(i=0;i<services_num;i++)
		{
			printf("====service %d=====\n",i);
			printf("Namespace:%s\n",s->Namespace);
			printf("XAddr:%s\n",s->XAddr);
			if(s->Capabilities)
			{
				printf("Capabilities:%s\n",s->Capabilities->__any==NULL?"NULL":s->Capabilities->__any);
			}
			if(s->Version)
			{
				printf("Version:Major(%d)Minor(%d)\n",s->Version->Major,s->Version->Minor);
			}
			if(s->__size>0)
			{
				int j;
				for(j=0;j<s->__size;j++)
				{
					printf("Any Info:\n");
					printf("Any:%s\n",s->__any==NULL?"NULL":s->__any);
					printf("Any Attr:%s\n",s->__anyAttribute==NULL?"NULL":s->__anyAttribute);
				}
			}
			s++;
		}
	}
	//get deviceinfo
	
	//SOAP_ENV__Header header;
	struct _tds__GetDeviceInformation information;
	struct _tds__GetDeviceInformationResponse informationResponse;
	//以摘要形式
	soap_wsse_add_UsernameTokenDigest(soap, "ID", "admin", "123456");  //以摘要形式
	//soap_wsse_add_UsernameTokenText(soap, "ID", "admin", "123456");	//以明文形式，有些IPC不支持,尽量用摘要形式
	ret = soap_call___tds__GetDeviceInformation(soap,
			"http://192.168.110.71/onvif/device_service",
			NULL,
			&information,
			&informationResponse);
	if(ret!=SOAP_OK)
	{
		soap_print_fault(soap,stderr);
		printf("=======GetDeviceInformation failed\n");
		goto ERR0;
	}
	else
	{
		printf("=======GetDeviceInformation successful\n");
	}
ERR0:
	soap_end(soap);
	soap_free(soap);
	return 0;
} 
