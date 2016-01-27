#create by liulu
#TARGET生成的程序名
TARGET = wsdd 
#TARGET + libdiscover.a
CONFIGURE = ./configure
P1 = p1.patch

#gsoap源码目录
GSOAP_DIR ?= ../gsoap-2.8
GSOAP_SDK = $(GSOAP_DIR)/gsoap

#依赖的目标文件，由对应的.c或.cpp生成
OBJ += main.o 
OBJ += event.o
OBJ += soapC.o
OBJ += soapClient.o  
#OBJ += soapServer.o  
#OBJ += soapServerLib.o
#OBJ += soapClientLib.o  


#直接使用源码文件链接，这样就不需要使用-lgsoap++
#OBJ += $(GSOAP_SDK)/stdsoap2.o
OBJ += $(GSOAP_SDK)/plugin/wsddapi.o
OBJ += $(GSOAP_SDK)/plugin/wsaapi.o

#用于交叉编译
#CROSS_COMPILE = arm-fsl-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc
CPP = $(CROSS_COMPILE)gcc
CFLAGS += -I.
CFLAGS += -I$(GSOAP_SDK)
#CPPFLAGS += -I.
#链接器，使用ld会有问题,使用g++
LD = $(CROSS_COMPILE)gcc
LDFLAGS = $(GSOAP_SDK)/libgsoap.a

all: $(TARGET)
$(TARGET): $(OBJ)
	$(LD) $^ $(LDFLAGS) -o $@
config:
	$(CONFIGURE)
	patch -p1 < $(P1)

#清除所有文件包括config生成的源文件
distclean:clean config_clean
#清除编译产生的.o文件
clean:
	rm $(OBJ) -rf	
	rm $(TARGET) -rf
#清除config产生的源文件
config_clean:
	$(CONFIGURE) clean
	
