#ifndef CLIENT
#define CLIENT

//C standard lib
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Posix
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>

//C++ 
#include <string>
#include <map>
using std::map;
using std::string;
#include "common.h"


typedef struct file_info{
	char f_name[PATH_MAX+1];
	int fd;
	off_t f_bytes;
	size_t f_blocks;
	size_t groups;	

}file_info;


//#ifdef DEBUG
#define TRACE_FILE_INFO(x) \
do{\
	printf("%s" ,(x)->f_name);\
	printf("%d ",(x)->fd);\
	printf("%d ",(x)->f_bytes);\
	printf("%d \n", (x)->f_blocks);\
}while(0)	
//#endif


int get_file_info(const char *filename, struct file_info * f_info); //获得本地文件的信息，用于encode

int write_vec_to_block(size_t blockid, struct VEC* vec);
int read_vec_from_file(int fd, struct VEC* vec);

int encode_file(struct file_info * f_info);
int decode_file(struct file_info * f_info);



#endif
