#create by liulu
#TARGET生成的程序名
SHELL = bash
CONFIGURE = ./configure
#依赖的模块
MODULE += gsoap-2.8
MODULE += discover 
MODULE += devicemgmt
MODULE += media


#用于交叉编译
#CROSS_COMPILE = arm-fsl-linux-gnueabi-

#编译工程,要先编译插件,因为有的模块会依赖插件
all:module $(TARGET)
	@echo "build complete!"

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
	$(foreach dir,$(MODULE),make -C $(dir);)
	@echo $@ complete!
module_distclean:
	@echo $@ ...
	$(foreach dir,$(MODULE),make -C $(dir) distclean;)
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

#清除config产生的源文件
config_clean:
	@echo $@ ...
	$(CONFIGURE) clean
	@echo $@ complete!
	
