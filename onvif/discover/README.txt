create by liulu
=====================================
功能:WS-Discover实现，用于探测当前网络内的IPC或提供Discover供客户端探测
======================================
目录结构            #
configure        #自动产生gsoap代码,被Makefile调用
Makefile            #编译脚本
p1.patch            #补丁文件
main.c              #测试程序
event.c             #probe resolve hello bye 事件处理器
======================================
使用方法            #
1.编译              #
make config         #产生gsoap源码
make                #编译
make clean          #清理编译产生的.o等
make distclean      #清理全部
