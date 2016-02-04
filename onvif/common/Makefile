#create by liulu
#TARGET生成的程序名
SHELL ?= bash
TARGET = libcommon.a
CONFIGURE = ./configure
#生成应用程序的依赖
OBJ += debug.o 
OBJ += mutex.o
OBJ += netSocket.o
OBJ += thread.o
#用于交叉编译
CROSS_COMPILE ?= arm-hisiv100nptl-linux-
CPP = $(CROSS_COMPILE)g++
CPPFLAGS += -I.
#链接器，使用ld会有问题,使用g++
LD = $(CROSS_COMPILE)g++
LDFLAGS = 
#归档器
AR = $(CROSS_COMPILE)ar
ARFLAGS = rcs

all: $(TARGET) sample
	@echo compile $(TARGET) successful
$(TARGET):$(OBJ)
#-Xlinker "-(" -Xlinker "-)" 解决编译库顺序依赖问题
#$(LD) $(LDFLAGS) -Xlinker "-(" $^ -Xlinker "-)" -o $@
	$(AR) $(ARFLAGS) $@ $^
sample:sample.cpp $(TARGET)
#for sample test
	$(CPP) -c -o sample.o sample.cpp
	$(LD) $(LDFLAGS) -Xlinker "-(" sample.o $(TARGET) -lpthread -Xlinker "-)" -o sample

%.o:%.cpp 
	$(CPP) -c -o $@ $^


config:
	$(CONFIGURE)

#清除所有文件包括config生成的源文件
distclean:clean config_clean
#清除编译产生的.o文件
clean:
	rm $(OBJ) -rf
	rm $(TARGET) -rf
	rm sample.o -rf
#清除config产生的源文件
config_clean:
	$(CONFIGURE) clean