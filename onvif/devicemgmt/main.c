#include "devicemgmtH.h"
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
ERR0:
	soap_end(soap);
	soap_free(soap);
	return 0;
} 
