#include "soapH.h"
// 必须包含nsmap文件，否则编译通不过
#include "MediaBinding.nsmap"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

int main()
{
	int ret;
	struct soap *soap = soap_new();
	struct _trt__GetServiceCapabilities capabilities;
	struct _trt__GetServiceCapabilitiesResponse capabilitiesResponse;
    printf("Media\n");
    ret = soap_call___trt__GetServiceCapabilities(soap,
    	"http://192.168.110.71/onvif/media_service", 
    	NULL, 
    	&capabilities, 
    	&capabilitiesResponse);
    if(ret!=SOAP_OK)
    {
		soap_print_fault(soap,stderr);
    	printf("GetServiceCapabilities fail\n");
    	goto ERR0;
    }
    else
    {
    	printf("GetServiceCapabilities successful\n");
    	if(capabilitiesResponse.Capabilities)
    	{
    		struct trt__Capabilities *c = capabilitiesResponse.Capabilities;
    		printf("capabilitiesResponse get\n");
    	}
    	else
    	{
    		printf("capabilitiesResponse is NULL\n");
    	}
    	
    }
ERR0:
	soap_end(soap);
	soap_free(soap);
	return 0;
} 
