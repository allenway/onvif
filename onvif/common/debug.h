#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>  

#define FI_PRINTF_LEVEL_1	    // 第一层调试
#define FI_PRINTF_LEVEL_2	    // 第二层调试        
      
#define STRERROR_ERRNO	( strerror(errno) )      

#ifdef __cplusplus
extern "C" {
#endif

void PrintHex( const char *message, unsigned char *data, int len, const char *format, ... );
void Print( const char *format, ... );
void PfDumpPrint( const char *file, int line, const char *format, ... );
void TeDumpPrint(const char *file, int line, const char *format, ... );

#ifdef __cplusplus
}
#endif

#ifdef FI_PRINTF_LEVEL_1  
#define SVPrint( format, arg... )    	PfDumpPrint( __FILE__, __LINE__, format, ##arg )
#else
#define SVPrint( format, arg... )
#endif

#ifdef FI_PRINTF_LEVEL_2  
#define FiPrint2( format, arg... )    	PfDumpPrint( __FILE__, __LINE__, format, ##arg )
#else
#define FiPrint2( format, arg... )
#endif

#define TePrint( format, arg... )    	TeDumpPrint( __FILE__, __LINE__, format, ##arg )






//for color PRINT sven
#define REDPRINTF       "\033[31m\033[1m"
#define GREENPRINTF     "\033[32m\033[1m"
#define YELLOWPRINTF    "\033[33m\033[1m"
#define BLUEPRINTF      "\033[34m\033[1m"
#define PURPLEPRINTF    "\033[35m\033[1m"
#define BTLGREENPRINTF  "\033[36m\033[1m"
#define NORMALPRINTF    "\033[0m"

#define COLORRED       1 //错误打印
#define COLORGREEN     2 //返回正确打印
#define COLORYELLOW    3
#define COLORBLUE      4
#define COLORPURPLE    5
#define COLORBTLGREEN  6

#ifdef FI_PRINTF_LEVEL_1  
#define ColorPrint(color,format, arg...) do{\
    switch(color){\
        case COLORRED:\
            Print("%s",REDPRINTF);\
            break;\
        case COLORGREEN:\
            Print("%s",GREENPRINTF);\
            break;\
        case COLORYELLOW:\
            Print("%s",YELLOWPRINTF);\
            break;\
        case COLORBLUE:\
            Print("%s",BLUEPRINTF);\
            break;\
        case COLORPURPLE:\
            Print("%s",PURPLEPRINTF);\
            break;\
        case COLORBTLGREEN:\
            Print("%s",BTLGREENPRINTF);\
            break;\
        default:\
            break;\
    }\
    PfDumpPrint( __FILE__, __LINE__, format, ##arg );\
    Print("%s",NORMALPRINTF);\
}while(0)
#else
#define ColorPrint(color,format, arg...)  
#endif


#ifdef FI_PRINTF_LEVEL_1  
#define ERRORPRINT(format, arg...) do{\
    Print("%s",REDPRINTF);\
    PfDumpPrint( __FILE__, __LINE__, format, ##arg );\
    Print("%s",NORMALPRINTF);\
}while(0)
#else
#define ERRORPRINT(format, arg...) 
#endif


#ifdef FI_PRINTF_LEVEL_1 
#define CORRECTPRINT(format, arg...) do{\
    Print("%s",GREENPRINTF);\
    PfDumpPrint( __FILE__, __LINE__, format, ##arg );\
    Print("%s",NORMALPRINTF);\
}while(0)
#else
#define CORRECTPRINT(format, arg...)  
#endif


#endif  

