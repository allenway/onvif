#create by liulu
#TARGET生成的程序名
SHELL = bash
TARGET = rhanvr
CONFIGURE = ./configure
#P1 = p1.patch
#gsoap源码目录
GSOAP_DIR ?= ../gosap-2.8
GSOAP_SDK = $(GSOAP_DIR)/gsoap
#依赖的目标文件，由对应的.c或.cpp生成
OBJ += main.o

#依赖的模块
MODULE += discover devicemgmt
#依赖的静态库
LIBA += discover/libdiscover.a
#依赖的动态库
LIBSO +=

#用于交叉编译
#CROSS_COMPILE = arm-fsl-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc
CPP = $(CROSS_COMPILE)g++
CFLAGS += -I. 
CFLAGS += -I$(GSOAP_SDK)
CPPFLAGS += -I. 
CPPFLAGS += -I$(GSOAP_SDK)

#链接器，使用ld会有问题,使用g++
LD = $(CROSS_COMPILE)g++

#编译工程
all:module $(TARGET)
	@echo "build complete!"

$(TARGET):$(LIBA) $(LIBSO) $(OBJ)
	$(LD) $(LIBA) $(LIBSO) $(OBJ)  -o $@
	


#编译模块
module_config:
#默认会使用make config去配置相应的模块
#如果对应的模块不支持make config命令，则会调用模块中configure
	@echo $@ ...
	@for d in $(MODULE); \
	do 					 \
		if [ -d $$d ];then 	 \
			echo config $$d ;   	\
			make -C $$d config ;    \
			ret=$$?;	\
			if [ $$ret -ne 0 ];then 	 \
				pushd .;cd $$d;./configure;ret=$$?;popd; \
			fi;  \
			if [ $$ret -ne 0 ];then 	 \
				echo config $$d error; \
				ret=$$?;	\
			else	\
				echo config $$d successful; \
			fi;  \
			echo config $$d complete ; \
		fi;  \
	done
	@echo $@ complete!
module:
	@echo $@ ...
	$(foreach dir,$(MODULE),make -C $(dir))
	@echo $@ complete!
module_distclean:
	@echo $@ ...
	$(foreach dir,$(MODULE),make -C $(dir) distclean)
	@echo $@ complete!
module_clean:
#gsoap目录不进行clean动作,以减少编译时间
	@echo $@ ...
	@for d in $(MODULE); \
	do 					 \
		if [ -d $$d ];then 	 \
			case $$d in 	 \
				#不清空gsoap目录,只在执行distclean才清空gsoap,节约编译时间 \
				gsoap*)	 	 \
					;;		 \
				*)			 \
					echo clean $$d ;   			\
					make -C $$d clean ;    		\
					echo clean $$d complete ;  	\
					;;	\
			esac	\
		fi  \
	done
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
	
