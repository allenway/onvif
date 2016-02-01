create by liulu
2016-1-27
=====================================
功能:HI3535 NVR 工程
======================================
目录结构            #
wsdl                #onvif参考wsdl文件，尽作参考使用
configure           #配置工程，产生make所需要的依赖文件
Makefile            #编译脚本
gsoap-2.8           #gsoap模块
discover            #WS-Discover功能模块
devicemgmt          #onvif device_service功能模块
======================================
使用方法            #
make config         #配置工程,相当于执行每个功能模块中./configure,但是对于有patch的模块，会进行patch动作
make                #编译
make clean          #清理编译产生的.o等
make distclean      #清理全部

如何向工程中添加模块?
    1>在Makefile中添加如下语句
    MODULE += <模块名字>
    2>在模块中创建Makefile文件,并实现make config;make;make clean;make distclean命令;
    对于make config命令相当于执行./configure,如果模块中cofigure文件可以不用提供此命令;
    对于不支持的命令,可以空操作代替
如何编译单独的模块?
    进入到模块所在的目录,进行相应的make指令

注意：
make config,会重新配置所有模块，有的模块需要联网（如：onvif相关模块）过程会比较久；
建议单独进入到目录中进行make config 或 ./configure 
