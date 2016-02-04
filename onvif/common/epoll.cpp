/********************************************************************************
**  Copyright (c) 2013, 深圳市动车电气自动化有限公司, All rights reserved.
**  author        :  sven
**  version       :  v1.0
**  date           :  2013.01.17
**  description  : 封装(wrap)epoll相关函数,
              使用本函数前请查看当前内核版本是否支持epoll.
********************************************************************************/
#include <unistd.h>
#include <string.h>
#include "epoll.h" 

/*******************************************************************************
* fn: 创建一个epoll
* size: 最大值
* 返回: 0 or -1
********************************************************************************/
int EpollCreat( int size )
{
	int ret;

	ret = epoll_create( size );
    
	return ret;
}

/*******************************************************************************
* fn: 关闭一个epoll
* epfd: EpollCreat 返回的值
*******************************************************************************/
int EpollClose( int epfd )
{
	int ret;

	ret = close( epfd );
    
	return ret;
}

/********************************************************************************
* fn: 控制epfd 对fd 的操作
* epfd: EpollCreat 返回的值
* op: EPOLL_CTL_*** 中的其中一个
* fd: 新添加的fd
* event: 将要对fd检测那些事件
* 返回: 0 or -1
********************************************************************************/
int EpollCtl( int epfd, int op, int fd, struct epoll_event *event )
{
	int ret;

	ret = epoll_ctl( epfd, op, fd, event );
    
	return ret;
}

/********************************************************************************
* fn: epoll 等待
* epfd: EpollCreat 返回的值
* events: out, 通常为一个数组,大小大概为20
* maxevents: 最大事件数
* timeout: 等待的时间(毫秒);-1,表示无限期等待;0,表示不等待
* 返回:0,没有socket准备好; > 0, 返回已经准备好了的socket的数目; -1,出错
*********************************************************************************/
int EpollWait( int epfd, struct epoll_event *events, int maxevents, int timeout )
{
	int ret;

	ret = epoll_wait( epfd, events, maxevents, timeout );
    
	return ret;
}

/* ===== 下面是我自己的封装 ===== */
/*******************************************************************************
* fn: 控制epfd 对fd 的add 操作
* epfd: EpollCreat 返回的值
* fd: 被控制的fd
* events: 将要对fd 操作哪些事件
* 返回: 0 or -1
********************************************************************************/
int EpollCtlAdd( int epfd, int sockfd, uint events )
{
	int ret;
	struct epoll_event ev = { 0 };
    
	ev.data.fd     = sockfd;
	ev.events     = events;
	ret = EpollCtl( epfd, EPOLL_CTL_ADD, sockfd, &ev );
    
	return ret;
}

/*******************************************************************************
* fn: 控制epfd 对fd 的del 操作
* epfd: EpollCreat 返回的值
* fd: 被控制的fd
* events: 将要对fd 操作哪些事件
* 返回: 0 or -1
*******************************************************************************/
int EpollCtlDel( int epfd, int sockfd, uint events )
{
	int ret;    
	struct epoll_event ev = { 0 };    
    
	ev.data.fd     = sockfd;
	ev.events     = events;
	ret = EpollCtl( epfd, EPOLL_CTL_DEL, sockfd, &ev );
    
	return ret;
}

/******************************************************************************
* fn: 控制epfd 对fd 的modify 操作
* epfd: EpollCreat 返回的值
* fd: 被控制的fd
* events: 将要对fd 操作哪些事件
* 返回: 0 or -1
******************************************************************************/
int EpollCtlMod( int epfd, int sockfd, uint events )
{
	int ret;
	static struct epoll_event ev;    
    
	ev.data.fd     = sockfd;
	ev.events     = events;
	ret = EpollCtl( epfd, EPOLL_CTL_MOD, sockfd, &ev );

	return ret;
}

