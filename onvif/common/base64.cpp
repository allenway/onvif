/*
 * base64.c -- Base64 Mime encoding
 *
 * Copyright (c) GoAhead Software Inc., 1995-2000. All Rights Reserved.
 *
 * See the file "license.txt" for usage and redistribution license requirements
 *
 * $Id: base64.c,v 1.1 2013/04/23 07:28:46 seven Exp $
 */

/******************************** Description *********************************/

/*
 *	The base64 command encodes and decodes a pSrc in mime base64 format
*/

/********************************* Includes ***********************************/

#include "debug.h"


#if 0
/******************************** Local Data **********************************/
/*
 *	Mapping of ANSI chars to base64 Mime encoding alphabet (see below)
 */

static short map64[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, 
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

static char	alphabet64[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/',
};

/*********************************** Code *************************************/
/*
 *	Decode a buffer from "pSrc" and into "pDst"
 */
/*
* fn: base64 编码
* pDst: out, 存放编码后的字符串
* pSrc: 需要编码的字串
* dstSize: pDst 所指向内存空间的长度
* 特别注意: pSrc 必须以'\0'结尾; 而且pDst 也是以'\0' 结尾
*/
int Base64Decode( char *pDst, char *pSrc, int dstSize )
{
	unsigned long	shiftbuf;
	char	        *cp, *op;
	int	        	c, i, j, shift;

	op = pDst;
    *op = '\0';
	cp = pSrc;
	while (*cp && *cp != '=') {
/*
 *    	Map 4 (6bit) input bytes and store in a single long (shiftbuf)
 */
    	shiftbuf = 0;
    	shift = 18;
    	for (i = 0; i < 4 && *cp && *cp != '='; i++, cp++) {
        	c = map64[*cp & 0xff];
        	if (c == -1) 
            {
            	SVPrint( "Bad pSrc: %s at |  %c  | index %d\r\n", pSrc, c, i);
            	return -1;
            } 
        	shiftbuf = shiftbuf | (c << shift);
        	shift -= 6;
        }
/*
 *    	Interpret as 3 normal 8 bit bytes (fill in reverse order).
 *    	Check for potential buffer overflow before filling.
 */
        --i;
    	if ((op + i) >= &pDst[dstSize]) 
        {
        	strcpy( pDst, "String too big" );
        	SVPrint( "%s:%s!\r\n", "size of pDst is too less" );
        	return -1;
        }
    	for (j = 0; j < i; j++) 
        {
            *op++ = (char) ((shiftbuf >> (8 * (2 - j))) & 0xff);
        }        
        *op = '\0';
    }
	return 0;
}
#else //new
/*******************************************************************************
Base64解码方法中，最简单的也是查表法：将64个可打印字符的值作为索引，查表得到的值（范围为0-63）依次连起来，拼凑成字节形式输出，就得到解码结果。 
********************************************************************************/
const char DeBase64Tab[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    62,        // '+'
    0, 0, 0,
    63,        // '/'
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61,        // '0'-'9'
    0, 0, 0, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,        // 'A'-'Z'
    0, 0, 0, 0, 0, 0,
    26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
    39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,        // 'a'-'z'
};
 
int Base64Decode( char* pDst, char* pSrc, int dstSize )
{
    int nDstLen;            // 输出的字符计数
    int nValue;             // 解码用到的长整数
    int myDeBase64Tab;
    int i;
    int nSrcLen = strlen( pSrc );
    i = 0;
    nDstLen = 0;
 
    // 取4个字符，解码到一个长整数，再经过移位得到3个字节
    while (i < nSrcLen)
    {
        if (*pSrc != '\r' && *pSrc!='\n')
        {
        	myDeBase64Tab = *pSrc++;
            nValue = DeBase64Tab[myDeBase64Tab] << 18;
            myDeBase64Tab = *pSrc++;
            nValue += DeBase64Tab[myDeBase64Tab] << 12;
            *pDst++ = (nValue & 0x00ff0000) >> 16;
            nDstLen++;
 
            if (*pSrc != '=')
            {
            	myDeBase64Tab = *pSrc++;
                nValue += DeBase64Tab[myDeBase64Tab] << 6;
                *pDst++ = (nValue & 0x0000ff00) >> 8;
                nDstLen++;
 
                if (*pSrc != '=')
                {
                	myDeBase64Tab = *pSrc++;
                    nValue += DeBase64Tab[myDeBase64Tab];
                    *pDst++ =nValue & 0x000000ff;
                    nDstLen++;
                }
            }
 
            i += 4;
        }
        else        // 回车换行，跳过
        {
            pSrc++;
            i++;
        }
     }
 
    // 输出加个结束符
    *pDst = '\0';
 
    return nDstLen;
}

#endif

#if 0
/******************************************************************************/
/*
 *	Encode a buffer from "pSrc" into "pDst"
 */
/*
* fn: base64 编码
* pDst: out, 存放解码后的字符串
* pSrc: 需要解码的字串
* dstSize: pDst 所指向内存空间的长度
* 特别注意: pSrc 必须以'\0'结尾; 而且pDst 也是以'\0' 结尾
*/
void Base64Encode( char *pDst, char *pSrc, int dstSize )
{
	unsigned long	shiftbuf;
	char	        *cp, *op;
	int	        	x, i, j, shift;

	op = pDst;
    *op = '\0';
	cp = pSrc;
    
	while (*cp) 
    {
/*
 *    	Take three characters and create a 24 bit number in shiftbuf
 */
    	shiftbuf = 0;
    	for (j = 2; j >= 0 && *cp; j--, cp++) {
        	shiftbuf |= ((*cp & 0xff) << (j * 8));
        }
/*
 *    	Now convert shiftbuf to 4 base64 letters.  The i,j magic calculates
 *    	how many letters need to be output.
 */
    	shift = 18;
    	for (i = ++j; i < 4 && op < &pDst[dstSize] ; i++) 
        {
        	x = (shiftbuf >> shift) & 0x3f;
            *op++ = alphabet64[(shiftbuf >> shift) & 0x3f];
        	shift -= 6;
        }
/*
 *    	Pad at the end with '='
 */
    	while (j-- > 0) 
        {
            *op++ = '=';
        }
        *op = '\0';
    }
}

#endif
//int Base64Encode( char *in, const int inLen, char *out, int outLen )
int Base64Encode( char *pDst, char *pSrc, int dstSize )
{
	static const char *codes ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	char *p = pDst;
	int inLen = strlen(pSrc);
	int times = inLen / 3;
	int i;
	int base64_len = 4 * ((inLen+2)/3);
    
	if(dstSize < base64_len)
    {
    	SVPrint( "Failed: dstSize(%d) < base64_len(%d)\r\n", dstSize, base64_len );
    	return -1;
    }

	for(i=0; i<times; ++i)
    {
        *p++ = codes[(pSrc[0] >> 2) & 0x3f];
        *p++ = codes[((pSrc[0] << 4) & 0x30) + ((pSrc[1] >> 4) & 0xf)];
        *p++ = codes[((pSrc[1] << 2) & 0x3c) + ((pSrc[2] >> 6) & 0x3)];
        *p++ = codes[pSrc[2] & 0x3f];
    	pSrc += 3;
    }
	if(times * 3 + 1 == inLen) 
    {
        *p++ = codes[(pSrc[0] >> 2) & 0x3f];
        *p++ = codes[((pSrc[0] << 4) & 0x30) + ((pSrc[1] >> 4) & 0xf)];
        *p++ = '=';
        *p++ = '=';
    }
	if(times * 3 + 2 == inLen) 
    {
        *p++ = codes[(pSrc[0] >> 2) & 0x3f];
        *p++ = codes[((pSrc[0] << 4) & 0x30) + ((pSrc[1] >> 4) & 0xf)];
        *p++ = codes[((pSrc[1] << 2) & 0x3c)];
        *p++ = '=';
    }
    *p = '\0';

	return 0;
}

