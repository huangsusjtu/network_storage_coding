#include <cstdio>
#include <cstdlib>
#include <cstring>

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

#include <map>
#include <iostream>
using namespace std;

#include "erasure.h"
#include "client.h"

int main()
{

/*	BYTE** res,**S;
	BIT **submatrix =NULL;
	int i,j;
	unsigned int pp=0;

	S = new BYTE*[6];
	for(i=0;i<6;i++)
	{
		S[i] = new BYTE [12];
		for(j=0;j<12;++j)
		{	srand(pp++);
			S[i][j] = (BYTE)rand()&((1<<8)-1);	
		}		
	}
	printf("数据矩阵\n");
	print_m(S,6,12);

	struct VEC invec;
	invec.base = S;
	invec.v_size = 6;
	invec.d_size = 12;
	
	
	struct VEC outvec;
	encode(&invec, &outvec);
	//print_m(invec.base,invec.v_size,invec.d_size);
	//cout<<endl;
	
	printf("\nEncoded\n");
	print_m(outvec.base,outvec.v_size,outvec.d_size);
	printf("\n");


	printf("random_select_k\n");
	int *index = random_select_k(10,6);
	print_vec(index,6);
	printf("\n");

	submatrix = new BIT*[6];
	for(i=0;i<6;i++)
	{
		submatrix[i] = &G[index[i]][0];
		invec.base[i] = outvec.base[index[i]];
	}	
	invec.v_size = 6;
	

	decode(&invec,&outvec,submatrix);
	print_m(invec.base,invec.v_size,invec.d_size);
	printf("\n");
	print_m(outvec.base,outvec.v_size,outvec.d_size);
	printf("\n");
*/

	int res;
	struct file_info f_info;
	res=get_file_info( "/home/huangsu/datablock/Random.pdf", &f_info );
	
	//TRACE(res);
	//TRACE_FILE_INFO(&f_info);

/*	struct VEC *invec,*outvec;
	invec = alloc_vec(CLOUMN, B_SIZE);
	outvec = alloc_vec(ROW, B_SIZE);
	print_m(invec->base,CLOUMN,B_SIZE);
	print_m(outvec->base,ROW,B_SIZE);
	
	destory_vec(invec);		
	destory_vec(outvec);	
*/	
	
	res = encode_file(&f_info);	
	//TRACE(res);
	
	
	res = decode_file(&f_info);
	TRACE(res);

	

	
	return 0;
}
/*
	for(int m=0;m<6;m++)
	{
		submatrix[m] = &G[*(id+1)][0];
	}	
*/

