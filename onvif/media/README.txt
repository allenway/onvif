create by liulu
=====================================
功能:Media实现,支持v10 v20两个版本
======================================
目录结构            #
configure           #自动产生gsoap代码,被Makefile调用
Makefile            #编译脚本
main.c              #测试程序
typemap.dat         #wsdl2h使用依赖文件,从gsoap安装目录拷贝
                    #新增tr2 = "http://www.onvif.org/ver20/media/wsdl"
======================================
使用方法            #
1.编译              #
make config         #产生gsoap源码
make                #编译
make clean          #清理编译产生的.o等
make distclean      #清理全部
