#ifndef __CONST_H__
#define __CONST_H__

typedef unsigned int 	uint;
typedef unsigned char 	uchar;
typedef unsigned short  ushort;  
typedef  char         	sharechar; // 用户ShareMalloc
typedef unsigned long long uint64_t;

#define	FI_SUCCESS	        	0	// 成功
#define	FI_FAIL	                (-1)    // 失败
#define FI_SUCCESSFUL         	FI_SUCCESS
#define FI_FAILED	        	FI_FAIL
#define	FI_TRUE	            	1	// 真
#define	FI_FALSE	        	0	// 假

#define NET_ADDRSIZE         	20	    // "192.168.188.168" 给20个bytes;"00:AB:BB:CC:BA:00" 共18个字节
#define MAX_PATH_LEVEL	    	20	    // 最大目录级数
#define COM_BUF_SIZE         	256	    // 常用buf 大小
#define MAX_CHANNEL_NUM	    	16	    // 最大通道数
#define DATETIME_LEN	    	20	    // "2013-11-18 10:20:28" 的长度
#define DATE_LEN	        	12	    // "2013-11-18" 的长度
#define TIME_LEN	        	12	    // "10:20:28" 的长度
#define NORMAL_USER_NAME_LEN	32      // 一般用户名的长度
#define NORMAL_PASSWD_LEN		32      // 一般密码的长度
#define NORMAL_URL_LEN	    	128     // 一般域名的长度
#define MAX_WEEK_DAY	    	7       //一周有7天
#define MAX_DAY_TIME_SEG		4	    //一天提供四个时间段选择
#define MAX_RECORD	            4
#define NORMAL_FILENAME_LEN		80	//

#define REAL_CHANNEL_NUM		4
#define CHANNEL_ENC_STREAM_NUM  2//每个通道的编码的最大流数

#define MAX_VENC_CHN_NUM (REAL_CHANNEL_NUM*2)
#define MAX_AENC_CHN_NUM  REAL_CHANNEL_NUM
#define MAX_ENC_CHN_NUM  (MAX_VENC_CHN_NUM+MAX_AENC_CHN_NUM)
#endif  

