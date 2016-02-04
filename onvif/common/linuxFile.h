#ifndef __LINUXFILE_H__
#define __LINUXFILE_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/vfs.h>
#include <dirent.h>

int Open( const char *pathname, int flags );
int Write( int fd, const void *buf, unsigned int count );
int Writen( int fd, void *buf, unsigned int size );
int Read( int fd, void *buf, unsigned int count );
int Readn( int fd, void *buf, unsigned int size );
int Readline( int fd, char *vptr, int maxlen );
void Close( int fd );
char *Strsep( char **stringp, const char *delim );
char *Strstr( char *haystack, char *needle );
int Strlen( const char *pstr );
int Unlink( const char *pathname );
int MakePath( char *newPath, int mode );
int Access( char *pathname, int mode );
int Unlink( char *pathname );
FILE *Fopen( char *pathname, char *mode );
int Fclose( FILE *fp );
int Fread( void *ptr, int size, int nmemb, FILE *stream );
int Fwrite( void *ptr, int size, int nmemb, FILE *stream );
int Fputs( char *s, FILE *stream );
void *Memmove( void *dest, const void *src, int n );
void *Memmem( const void *haystack, int haystacklen,
                const void *needle, int needlelen );
int Strcmp(const char *s1, const char *s2);
int Strncmp(const char *s1, const char *s2, int n);
char *Strcat(char *s1, const char *s2);
void *Memset(void *s, int c, int n);
char *Strncpy( char *dest, const char *src, int n );
char *Strcpy( char *dest, const char *src);
void *Memcpy( void *dest, const void *src, int n );
int Usleep( uint usec );
int Ssleep( uint sec );
int Statfs( const char *path, struct statfs *buf );
int Lstat( const char *path, struct stat *buf );
int Closedir( DIR *dir );
int Chmod( const char *path, unsigned int mode );
int Rmdir( const char *pathname );
DIR *Opendir( const char *name );
struct dirent *Readdir( DIR *dir );
int Mkdir( const char *pathname, uint mode );
int Mount( const char *source, const char *target,
                 const char *filesystemtype, unsigned long mountflags,
                 const void *data );
int Umount( const char *target );
int Strcasecmp( const char *s1, const char *s2 );
int Fsync(int fd);

#endif 

