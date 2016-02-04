/********************************************************************************
**  Copyright (c) 2013, 深圳市动车电气自动化有限公司, All rights reserved.
**  author        :  sven
**  version       :  v1.0
**  date           :  2013.01.17
**  description  : 封装epoll相关函数,使用本函数前请查看当前内核版本是否支持epoll
********************************************************************************/

#ifndef __EPOLLW_H__
#define __EPOLLW_H__

#include <sys/epoll.h>

int EpollCreat( int size );
int EpollClose( int epfd );
int EpollCtl( int epfd, int op, int fd, struct epoll_event *event );
int EpollWait( int epfd, struct epoll_event *events, int maxevents, int timeout );

/* ===== 下面是我自己的封装 ===== */
int EpollCtlAdd( int epfd, int sockfd, uint events );
int EpollCtlDel( int epfd, int sockfd, uint events );
int EpollCtlMod( int epfd, int sockfd, uint events );


// 下面这些宏、类型都在<sys/epoll.h>中定义,放在这里仅仅为了识别方便
#ifdef _sven_EPOLLW_ 
typedef union epoll_data 
{
    void *ptr;
    int fd;                // 需要epoll的fd
    __uint32_t u32;
    __uint64_t u64;
} epoll_data_t;
struct epoll_event 
{
    __uint32_t events;      /* Epoll events,见下面 EPOLL*** 的或组合 */
    epoll_data_t data;      /* User data variable */
};

// epoll 事件
#define EPOLLIN         //表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
#define EPOLLOUT	    //表示对应的文件描述符可以写；
#define EPOLLPRI	    //表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
#define EPOLLERR	    //表示对应的文件描述符发生错误；
#define EPOLLHUP	    //表示对应的文件描述符被挂断；
#define EPOLLET	        //将#define EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
#define EPOLLONESHOT	//只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个

/* Valid opcodes ( "op" parameter ) to issue to epoll_ctl().  */
#define EPOLL_CTL_ADD 1	/* Add a file decriptor to the interface.  */
#define EPOLL_CTL_DEL 2	/* Remove a file decriptor from the interface.  */
#define EPOLL_CTL_MOD 3	/* Change file decriptor epoll_event structure.  */

int epoll_create( int size );
int epoll_ctl( int epfd, int op, int fd, struct epoll_event *event );
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
#endif 

#endif 

