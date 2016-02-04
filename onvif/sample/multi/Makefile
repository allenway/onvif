#create by liulu
#TARGET生成的程序名
TARGET = wsdd 
TARGET += libdiscover.a 
CONFIGURE = ./configure
P1 = p1.patch

#gsoap源码目录
GSOAP_DIR ?= ../gsoap-2.8
GSOAP_SDK = $(GSOAP_DIR)/gsoap

#生成应用程序的依赖
OBJ_A += main.o 
OBJ_A += ../plugin/wsddapi.o
OBJ_A += ../plugin/wsaapi.o
OBJ_A += discoverC.o
OBJ_A += discoverClient.o
OBJ_A += event.o
#生成库的依赖
OBJ_L += event.o
#OBJ_L += discoverC.o
#OBJ_L += discoverClient.o  
#OBJ_L += discoverServer.o  
#OBJ_L += discoverServerLib.o
OBJ_L += discoverClientLib.o  




#用于交叉编译
#CROSS_COMPILE = arm-fsl-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc
CPP = $(CROSS_COMPILE)gcc
CFLAGS += -I.
CFLAGS += -I$(GSOAP_SDK)
#CPPFLAGS += -I.
#链接器，使用ld会有问题,使用g++
LD = $(CROSS_COMPILE)gcc
LDFLAGS = 
#归档器
AR = $(CROSS_COMPILE)ar
ARFLAGS = rcs

all: $(TARGET)
	@echo compile $(TARGET) successful
#wsdd:$(OBJ_A) libdiscover.a $(GSOAP_SDK)/libgsoap.a
wsdd:$(OBJ_A)  $(GSOAP_SDK)/libgsoap.a
#-Xlinker "-(" -Xlinker "-)" 解决编译库顺序依赖问题
	$(LD) $(LDFLAGS) -Xlinker "-(" $^ -Xlinker "-)" -o $@
libdiscover.a:$(OBJ_L)
	$(AR) $(ARFLAGS) $@ $^
config:
	$(CONFIGURE)
	patch -p1 < $(P1)

#清除所有文件包括config生成的源文件
distclean:clean config_clean
#清除编译产生的.o文件
clean:
	rm $(OBJ_A) -rf
	rm $(OBJ_L) -rf
	rm $(TARGET) -rf
#清除config产生的源文件
config_clean:
	$(CONFIGURE) clean
	
