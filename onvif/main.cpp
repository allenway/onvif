//create by liulu
//NVR入口函数

#include<stdio.h>
#include<unistd.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include"discover/soapH.h"

#include "nvr.nsmap"

int main(void)
{
    printf("=====rhanvr start=====\n");
    struct soap *soap = soap_new1(SOAP_IO_UDP);
    int loop = 1;
    while(loop)
    {
        printf("running ...\n");
        sleep(5);
    }
    printf("=====rhanvr stop======\n");
    soap_end(soap);
    soap_free(soap);
    return 0;
}
