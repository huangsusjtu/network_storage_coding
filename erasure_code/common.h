#ifndef COMMON
#define COMMON

#include "erasure.h"
#include "client.h"

//
#define ERR_OpenFile -1
#define ERR_StatFile -2
#define ERR_FileType -3
#define ERR_Pointer -4
#define ERR_File -5
#define Err_Read -6
//

//
#define B_OFFSET 12
#define B_MASK   (1<<B_OFFSET)-1L
#define B_SIZE	 1<<B_OFFSET

//

typedef unsigned int BIT; 
typedef unsigned char BYTE;


#define foreach(i, c) for(__typeof((c).begin()) i = (c).begin(); i != (c).end(); ++i)
#define TRACE(x) std::cout<<#x<<" = "<<x<<std::endl


struct VEC* alloc_vec(size_t v, size_t d);//辅助函数 分配一个VEC
void destory_vec(struct VEC* vec);//辅助函数 释放一个VEC


#endif
