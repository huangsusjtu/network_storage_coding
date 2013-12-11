#ifndef ERASURE_
#define ERASURE_
//C standard lib
#include <cstdio>
#include <cstdlib>
#include <cstring>

//Posix
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <limits.h>

//C++ 
#include <iostream>
#include <string>
using std::string;

#include "common.h"

typedef unsigned int BIT; 
typedef unsigned char BYTE;




extern BIT G[][6];
extern const size_t R;
extern const size_t CLOUMN;
extern const size_t ROW ;
//获得全0矩阵
#define ZEROS_MATRIX(res,N1,N2,TYPE)					\
do{											\
	res =  new TYPE*[N1];					\
	for(int i=0;i<(N1);i++)					\
	{										\
		res[i] = new TYPE[N2];				\
		memset(res[i], 0, sizeof(TYPE)*N2);  \
	}										\
}while(0)
//释放矩阵
#define FREE_MATRIX(res,N1,N2)					\
do{								\
	for(int i=0;i<(N1);i++)					\
	{										\
		delete [] res[i];\
	}	\
	delete [] res;						\
}while(0)


//矩阵的行相减
#define MINUS_VEC( p1, p2, b, n, it)      	\
do{                                   		 \
	for(it=b;it<n;++it)               	 \
		p1[it] = p1[it]^p2[it];       	 \
}while(0)                             

//输出矩阵
#define print_m(res,n1,n2) \
do{ \
	int w,m;\
	for(w=0;w<n1;w++)\
	{\
		for(m=0;m<n2;m++)\
		{\
			printf("0x%02x",res[w][m]);	\
		}\
		printf("\n");\
	}	\
}while(0)
      
//输出vec
#define print_vec(res,n) \
do{ \
	int w;\
	for(w=0;w<n;w++)\
	{\
		printf("%d ",res[w]);\
	}	\
	printf("\n");\
}while(0)
                     
//向量的2进制相加
#define ADD_VEC(a, b, bytes)\
do{\
	__typeof(a)  s1=(a);\
	__typeof(a)  s2=(b);\
	for(int i=0;i<bytes;++i)\
	{\
		*s1 = *s1^*s2;\
		s1++;\
		s2++;\
	}\
}while(0)


struct VEC{   //描述一组数据块
	BYTE** base;
	size_t v_size;
	size_t d_size;
};


int encode(const struct VEC* in_vec , struct VEC* out_vec); //S = D*G, out_vec的空间未分配
int decode(const struct VEC* in_vec , struct VEC* out_vec, BIT** submatrix); //给定子矩阵 
int decode_direct(const struct VEC* in_vec, struct  VEC* out_vec ,const BIT** inv_submatrix);//给定子矩阵的逆

int* random_select_k(int n,int k);//辅助函数生成 在[0,n]任意取K数
 BIT** inv(BIT** S,int n); //矩阵求逆



#endif
