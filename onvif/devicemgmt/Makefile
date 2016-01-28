#create by liulu
#TARGET生成的程序名
TARGET = mgmt
CONFIGURE = ./configure
#P1 = p1.patch

#依赖的目标文件，由对应的.c或.cpp生成
OBJ_A += main.o
OBJ_L += devicemgmtC.o
OBJ_L += devicemgmtClient.o  
#OBJ += devicemgmtServer.o  
#OBJ += devicemgmtServerLib.o
#OBJ += devicemgmtClientLib.o  

#gsoap源码目录
GSOAP_DIR ?= ../gsoap-2.8
GSOAP_SDK = $(GSOAP_DIR)/gsoap
#直接使用源码文件链接，这样就不需要使用-lgsoap++
OBJ_A += $(GSOAP_SDK)/stdsoap2.o
OBJ_A += ../custom/duration.o

#用于交叉编译
#CROSS_COMPILE = arm-fsl-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc
CPP = $(CROSS_COMPILE)gcc
CFLAGS += -I. 
CFLAGS += -I$(GSOAP_DIR)
#CPPFLAGS += -I.
#链接器，使用ld会有问题,使用g++
LD = $(CROSS_COMPILE)gcc

all: $(TARGET)
$(TARGET): $(OBJ_A) $(OBJ_L)
	$(LD) $^  -o $@
config:
	$(CONFIGURE)
ifneq ($(P1),)
	patch -p1 < $(P1)
endif
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
	
