#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>C
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: %s ip port", argv[0]);
        exit(1);
    }
    printf("This is a UDP client\n");
    struct sockaddr_in addr;
    int sock;

    if ( (sock=socket(AF_INET, SOCK_DGRAM, 0)) <0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        printf("Incorrect ip address!");
        close(sock);
        exit(1);
    }

    char buff[5120];
    char buff1[5120] = {0};
    int len = sizeof(addr);
    while (1)
    {


		if ( (sock=socket(AF_INET, SOCK_DGRAM, 0)) <0)
		{
			perror("socket");
			exit(1);
		}
		addr.sin_family = AF_INET;
		addr.sin_port = htons(atoi(argv[2]));
		addr.sin_addr.s_addr = inet_addr(argv[1]);
		if (addr.sin_addr.s_addr == INADDR_NONE)
		{
			printf("Incorrect ip address!");
			close(sock);
			exit(1);
		}


        gets(buff1);
        int n;
        n = sendto(sock, buff1, strlen(buff1), 0, (struct sockaddr *)&addr, sizeof(addr));
        if (n < 0)
        {
            perror("\n sendto \n");
            close(sock);
            break;
        }
        printf("send successful\n");
        n = recvfrom(sock, buff, 5120, 0, (struct sockaddr *)&addr, &len);
        if (n>0)
        {
            buff[n] = 0;
            printf("received:\n");
            puts(buff);
        }
        close(sock);
   #if 0
        else if (n==0)
        {
            printf("server closed\n");
            close(sock);
            break;
        }
        else if (n == -1)
        {
            perror("recvfrom");
            close(sock);
            break;
        }
    #endif
    }

    return 0;
}

