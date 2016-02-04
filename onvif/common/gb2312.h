#ifndef __GB2312_H__
#define __GB2312_H__

int Gb2312ToUtf8( char *pGb2312, int len, char **ppUtf8 );
void Gb2312ToUnicode( unsigned short* pOut, char *pGb2312Word );

#endif 

