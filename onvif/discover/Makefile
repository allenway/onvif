#create by liulu
#TARGET生成的程序名
TARGET = wsdd 
CONFIGURE = ./configure

#gsoap源码目录
GSOAP_DIR ?= ../gsoap-2.8
GSOAP_SDK = $(GSOAP_DIR)/gsoap

#生成应用程序的依赖
OBJ += main.o 
OBJ += $(GSOAP_SDK)/plugin/wsddapi.o
OBJ += $(GSOAP_SDK)/plugin/wsaapi.o
OBJ += soapC.o
OBJ += soapClient.o
OBJ += event.o
#OBJ += soapServer.o  
#OBJ += soapServerLib.o
#OBJ += soapClientLib.o  


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
$(TARGET):$(OBJ)  $(GSOAP_SDK)/libgsoap.a
#-Xlinker "-(" -Xlinker "-)" 解决编译库顺序依赖问题
	$(LD) $(LDFLAGS) -Xlinker "-(" $^ -Xlinker "-)" -o $@
config:
	$(CONFIGURE)

#清除所有文件包括config生成的源文件
distclean:clean config_clean
#清除编译产生的.o文件
clean:
	rm $(OBJ) -rf
	rm $(TARGET) -rf
#清除config产生的源文件
config_clean:
	$(CONFIGURE) clean
	
