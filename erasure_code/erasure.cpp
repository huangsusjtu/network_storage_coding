#include "erasure.h"

const size_t R = 6; //矩阵秩
BIT G[][6]={             //生成矩阵
{1,0,0,0,0,0},
{0,1,0,0,0,0},
{0,0,1,0,0,0},
{0,0,0,1,0,0},
{0,0,0,0,1,0},
{0,0,0,0,0,1},
{0,1,0,1,0,1},
{1,0,1,0,1,0},
{1,1,1,0,0,1},
{0,1,1,1,1,0},
};
const size_t CLOUMN = 6; 
const size_t ROW = 10 ;

BIT H[4][10]={    //校验矩阵
{0,1,1,1,1,0,0,0,0,1},
{1,0,0,1,1,1,0,0,1,0},
{0,1,0,0,0,1,0,1,0,0},
{1,0,1,0,1,0,1,0,0,0}
};

 BIT** inv(BIT** S, int n) //n*n的矩阵求逆 ，
{
	BIT** res;
	int i,j,k,it;
	ZEROS_MATRIX(res,n,n,BIT);
	for( i=0;i<n;++i)
		res[i][i]=1;
	
	
	for( i=0; i<n;++i) //i为列
	{
		for( j=i;j<n;++j) //j为行
		{
			if(S[j][i])
				break;
		}
		
		if(n==j)
			continue;
		for(k=0; k<n; ++k)
		{
			
			if( j!=k && S[k][i] )
			{	
				MINUS_VEC(S[k], S[j], i,n, it);
				MINUS_VEC(res[k], res[j], 0,n, it);
			}
		}
		if(i!=j)
		{
			BIT* tmp = res[i];
			res[i] = res[j];
			res[j] = tmp;
			tmp = S[i];
			S[i] = S[j];
			S[j] = tmp;
		}
	}
	return res;
}

int encode(const struct VEC *in_vec, struct VEC* out_vec) //数据编码
{
		if(!in_vec || !out_vec )
				return ERR_Pointer;
		BYTE* a =NULL;			

		for(int i=0;i<ROW;++i)  //G的每一行
		{
			a = out_vec->base[i];
			memset(a,0,in_vec->d_size);
		
			for(int j=0; j<CLOUMN;j++)  //该行的每一个元素
			{
				if(G[i][j])
				{
					ADD_VEC(a, (in_vec->base)[j] , in_vec->d_size );
				}
			}

			out_vec->base[i] = a;
		}	
		return 0;							
}

int decode_direct(const struct VEC* in_vec, struct  VEC* out_vec ,const BIT** inv_submatrix)
{
	if(!in_vec || !out_vec || !inv_submatrix)
		return ERR_Pointer;
	int i,j;
	size_t dsize = in_vec->d_size;
	
	
	BYTE* a =NULL;

	for(i=0; i<CLOUMN ; ++i)
	{
		a = out_vec->base[i];
		memset(a,0,dsize);

		for(j=0; j<CLOUMN; ++j)
		{
			if(inv_submatrix[i][j])
			{
				ADD_VEC(a, (in_vec->base)[j] , dsize);
			}			
		}
		out_vec->base[i] = a;
	}	
}

int decode(const struct VEC* in_vec, struct  VEC* out_vec , BIT** submatrix)
{
	if(!in_vec || !out_vec || !submatrix)
		return ERR_Pointer;
	size_t i,j;
	size_t dsize = in_vec->d_size;
	BIT **inv_submatrix = NULL;	
	
	BYTE* a = NULL;

	
	inv_submatrix = inv(submatrix, CLOUMN);
	
	for(i=0; i<CLOUMN ; ++i)
	{
		a = out_vec->base[i];
		memset(a,0,dsize);

		for(j=0; j<CLOUMN; ++j)
		{
			if(inv_submatrix[i][j])
			{
				ADD_VEC(a, (in_vec->base)[j] , dsize);
			}			
		}
		out_vec->base[i] = a;
	}
	FREE_MATRIX(inv_submatrix,CLOUMN,CLOUMN);
	return 0;	
}

//辅助函数
int* random_select_k(int n,int k)
{
	int i,pos=0,r,n1;
	int *d;

	if(n<=0 || k<=0 || n<n)
		return NULL;
	n1 = n;	
	d = new int [k];
	srand(i);
	for(i=0;i<n ;i++)
	{	
		
		r = rand()%(n-i);
		if(r<k)
		{	
			d[pos++] = i;
			k--;
		}
	}
	return d;
}























