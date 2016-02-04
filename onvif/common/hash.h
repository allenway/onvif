#ifndef __HASH_H__
#define __HASH_H__

typedef struct _HashSt_ 
{
	char *pKey;
	void *pVal;
	struct _HashSt_ *next;

} HASH_T;

class ClHash
{
public:
	ClHash( int maxNode );
    ~ClHash();    
	int insert( char *pKey, void *pVal, int valSize );
	int remove( char *pKey );
	void *GetVal( char *pKey );
private:
	char *StrCopy( const char *pStr );    
	void FreeNode( HASH_T *pNode );
private:
	HASH_T **m_ppTable;
	int		m_maxNode;
};


int ElfHash( char *key, int max );

#endif  

