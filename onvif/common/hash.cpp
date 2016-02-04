/********************************************************************************
**  Copyright (c) 2013, 深圳市动车电气自动化有限公司, All rights reserved.
**  author        :  sven
**  version       :  v1.0
**  date           :  2013.10.10
**  description  : 封装hash算法接口
********************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "hash.h"
/*
* fn: 字符形式的hash算法封装, H = H(key)
* key: 用来计算hash 值的关键字
* max: hash的成员格数,index的个数
* 返回: 由key 经过算法产生的hash值,
*/
int ElfHash( char *key, int max )
{
	unsigned int h = 0;
	unsigned int g;
    
	while( *key )
    {
    	h = (h<<4) + *key++;
    	g = h&0xF0000000;
    	if( g )
    	h ^= g>>24;
    	h &= ~g;
    }
    
	return h%max;
}

ClHash::ClHash( int n )
{
	int tableSize;
    
	m_maxNode      = n;
	tableSize	= sizeof(m_ppTable) * m_maxNode;
	m_ppTable     = (HASH_T **)malloc( tableSize );    
	if( NULL != m_ppTable )
    {
    	memset( m_ppTable, 0x00, tableSize );
    }
	else
    {
    	m_maxNode = 0;
    }
}

ClHash::~ClHash()
{
	int index;
	HASH_T *pTable, *pNext;
    
	if ( m_ppTable != NULL )
    {
    	for( index = 0; index < m_maxNode; index++ )
        {            
        	pTable = m_ppTable[index];
        	while( pTable != NULL )
            {
            	pNext = pTable->next;
            	FreeNode( pTable );                
            	pTable = pNext;
            }
        	m_ppTable[index] = NULL;
        }

    	free( m_ppTable );
    }
}

void ClHash::FreeNode( HASH_T *pNode )
{
	if( NULL != pNode )
    {
    	free( pNode->pKey );
    	free( pNode->pVal );
    	free( pNode );
    }
}

/* 字符串拷贝函数，动态分配内存保存拷贝的字符串 */
char *ClHash::StrCopy( const char *pStr ) 
{
	int len = strlen( pStr );
	char *pBuf = ( char * )malloc( sizeof( char ) * ( len + 1 ) );
	if ( pBuf != NULL ) strcpy( pBuf, pStr );
	return pBuf;
}

int ClHash::insert( char *pKey, void *pVal, int valSize )
{
	int ret = -1, index;
	HASH_T *pTable;

	if ( NULL != m_ppTable && NULL != pKey && NULL != pVal )
    {
    	index = ElfHash( pKey, m_maxNode );
    	for( pTable = m_ppTable[index]; pTable != NULL; pTable = pTable->next )
        {
        	if( 0 == strcmp(pTable->pKey, pKey) )
            {
            	ret = 0;
            	break;
            }
        }
    	if( -1 == ret ) // 哈希表里面不存在该key,则插入
        {
        	pTable = (HASH_T *)malloc( sizeof(HASH_T) );
        	if( NULL != pTable )
            {
            	pTable->pKey = StrCopy( pKey );
            	if( NULL == pTable->pKey )
                {
                	free( pTable );
                }
            	else
                {
                	pTable->pVal = malloc( valSize );
                	if( NULL == pTable->pVal )
                    {
                    	free( pTable->pKey );
                    	free( pTable );
                    }
                	else /* 插入成功 */
                    {
                    	memcpy( pTable->pVal, pVal, valSize );
                    	pTable->next = m_ppTable[index];
                    	m_ppTable[index] = pTable;
                    	ret = 0;
                    }
                }
            } // if( NULL != pTable
        }
    } // if ( NULL != m_ppTable && NULL != pKey 

	return ret;
}

void *ClHash::GetVal( char *pKey )
{
	HASH_T *pTable = NULL;
    
	if ( m_ppTable != NULL && pKey != NULL )
    {
    	int index = ElfHash( pKey, m_maxNode );
    	for ( pTable = m_ppTable[index]; pTable != NULL; pTable = pTable->next )
        {
        	if ( strcmp( pTable->pKey, pKey ) == 0 )
            	return pTable->pVal;
        }
    }

	return NULL;
}


int ClHash::remove( char *pKey )
{
	int ret = -1, index;
	HASH_T *pPreTable, *pCurTable;
    
	if ( m_ppTable != NULL && pKey != NULL )
    {
    	index = ElfHash( pKey, m_maxNode );            
    	pPreTable = m_ppTable[index];
    	pCurTable = pPreTable->next;
        
    	if ( strcmp( m_ppTable[index]->pKey, pKey ) == 0 ) // 头节点的key就等于要删除节点的key
        {
        	pCurTable = m_ppTable[index];
        	m_ppTable[index] = m_ppTable[index]->next;
        	FreeNode( pCurTable );            
        	ret = 0;
        }        
    	else
        {
        	pPreTable = m_ppTable[index];
        	pCurTable = pPreTable->next;
        	while ( pCurTable != NULL )
            {
            	if ( strcmp( pCurTable->pKey, pKey ) == 0 )
                {
                	pPreTable->next = pCurTable->next;
                	FreeNode( pCurTable );
                	ret = 0;
                	break;
                }

            	pPreTable = pCurTable;
            	pCurTable = pCurTable->next;
            }
        }        
    } // if ( m_ppTable != NULL && pKey != NULL
	return ret;    
}



