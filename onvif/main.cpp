//create by liulu
//NVR入口函数

#include<stdio.h>
#include<iostream>
#include <unistd.h>
int main(void)
{
    printf("=====rhanvr start=====\n");
    int loop = 1;
    while(loop)
    {
        printf("running ...\n");
        sleep(5);
    }
    printf("=====rhanvr stop======\n");
    return 0;
}
