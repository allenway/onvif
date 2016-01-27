#create by liulu
#TARGET生成的程序名
TARGET = rhanvr
CONFIGURE = ./configure
#P1 = p1.patch
#gsoap源码目录
GSOAP_DIR = ../gosap-2.8/gsoap

#依赖的目标文件，由对应的.c或.cpp生成
OBJ += main.o

#依赖的模块
MODULE += discover
#依赖的静态库
LIBA += discover/libdiscover.a
#依赖的动态库
LIBSO +=

#用于交叉编译
#CROSS_COMPILE = arm-fsl-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc
CPP = $(CROSS_COMPILE)g++
CFLAGS += -I. 
CFLAGS += -I$(GSOAP_DIR)
CPPFLAGS += -I. 
CPPFLAGS += -I$(GSOAP_DIR)

#链接器，使用ld会有问题,使用g++
LD = $(CROSS_COMPILE)g++

#编译工程
all:module $(TARGET)
	@echo "build complete!"

$(TARGET):$(LIBA) $(LIBSO) $(OBJ)
	$(LD) $(LIBA) $(LIBSO) $(OBJ)  -o $@
	


#编译模块
module_config:
	@echo $@ ...
	@echo $@ complete!
module:
	@echo $@ ...
	@echo $@ complete!
module_distclean:
	@echo $@ ...
	@echo $@ complete!
module_clean:
	@echo $@ ...
	@echo $@ complete!

#清除所有文件包括config生成的源文件
distclean:clean module_distclean config_clean
	@echo $@ ...
	@echo $@ complete!

#清除编译产生的.o文件
clean:module_clean
	@echo $@ ...
	rm $(OBJ) -rf	
	rm $(TARGET) -rf
	@echo $@ complete!
	
#配置工程
config:module_config
	@echo $@ ...
	$(CONFIGURE)
#如果有补丁文件，需要对源码进行打补丁
ifneq ($(P1),)
	patch -p1 < $(P1)
endif
	@echo $@ complete!

#清除config产生的源文件
config_clean:
	@echo $@ ...
	$(CONFIGURE) clean
	@echo $@ complete!
	
