#include "client.h"


//获得文件信息，用于分片
 int get_file_info(const char *filename, struct file_info * f_info)
{	
	
	int fd;
	struct stat attribute;
	if(lstat(filename, &attribute) < 0)
		return ERR_StatFile;
	if( !S_ISREG(attribute.st_mode))
		return ERR_FileType;
	if( attribute.st_size == 0)
		return ERR_File;
	fd = open(filename , O_RDWR | O_SYNC);
	if(fd < 0 )
		return ERR_OpenFile;
	
	
	f_info->fd = fd;
	strcpy(f_info->f_name,filename);
	f_info->f_bytes = attribute.st_size;
	f_info->f_blocks = (attribute.st_size>>B_OFFSET) + (0< attribute.st_size&B_MASK);
	return 0;
}



//从文件读取一个vec的数据，数据不够的话，用随机数据。
 int read_vec_from_file(int fd, struct VEC* vec)
{
	if(!vec)
		return ERR_Pointer;
	
	int c,read_size;
	for(c=0; c < vec->v_size; ++c)
	{
		read_size = read( fd, vec->base[c] , vec->d_size );
		if(read_size<0)
			return Err_Read;
		if(read_size < vec->d_size )	
			break;
	}
	return vec->v_size;
}

static char path[PATH_MAX+1];
 int write_vec_to_block(size_t blockid, struct VEC* vec)
{
	
	sprintf(path,"/home/huangsu/datablock/%d",blockid);
	int fd = open( path, O_WRONLY | O_CREAT | O_TRUNC, 0755);
	if(fd<0)
		return ERR_OpenFile;
	for(size_t i =0; i<vec->v_size; ++i)
	{
		write(fd, vec->base[i] , vec->d_size );
		fsync(fd);
	}
	close(fd);
	return vec->v_size;
}


 int encode_file(struct file_info * f_info)
{
	if( !f_info )
		return ERR_Pointer;
	int res;
	struct VEC* invec = NULL;
	struct VEC* outvec = NULL;
	size_t groups = (f_info->f_blocks)/CLOUMN;
	
	if( groups*CLOUMN < f_info->f_blocks)	
		groups++;
	f_info->groups = groups;

	//TRACE(groups);
	//TRACE(f_info->f_bytes);
	//TRACE(f_info->f_blocks);

	invec = alloc_vec(CLOUMN, B_SIZE);
	outvec = alloc_vec(ROW, B_SIZE);	

	for(size_t i=0;i<groups;i++)
	{
		res = read_vec_from_file(f_info->fd, invec);
		//TRACE(res);
		res = encode(invec, outvec);	
		//TRACE(res);

		write_vec_to_block(i+1, outvec);
	}

	destory_vec(invec);
	destory_vec(outvec);
	return  groups;
}	


 int decode_file(struct file_info * f_info)
{
	
	int res,file;
	size_t writed =0;
	struct VEC* invec = NULL;
	struct VEC* outvec = NULL;
	BIT **submatrix =NULL;

	outvec = alloc_vec(CLOUMN, B_SIZE);
	invec = alloc_vec(ROW, B_SIZE);	
	
	mkdir("/home/huangsu/datablock/",0755);
	strcpy(path,"/home/huangsu/datablock/regeneratefile");
	int fd1 = open( path, O_WRONLY | O_CREAT | O_APPEND | O_SYNC, 0755);
	if(fd1<0)
		return ERR_OpenFile;

	//int *index = random_select_k(10,6);
	//print_vec(index,6);
	submatrix = new BIT*[6];
	for(int m=0;m<6;m++)
	{
		submatrix[m] = &G[m][0];
	}	 // 初始化用于解码的子矩阵
	submatrix[5] = &G[6][0];
	BIT ** inv_submatrix = inv(submatrix, CLOUMN);
	


	struct VEC invec1;
	invec1.base = new BYTE* [6];
	invec1.v_size = 6;
	invec1.d_size = invec->d_size;
	for(int m=0;m<6;m++)
	{
		invec1.base[m] = invec->base[m];
	}	//获得用于解码的数据
	invec1.base[5] = invec->base[6];


	for(size_t i=0;i<f_info->groups;i++)
	{	
		
		/*strcpy(path,"/home/huangsu/datablock/random1");
		int fd1 = open( path, O_WRONLY | O_CREAT | O_APPEND | O_SYNC, 0755);
		if(fd1<0)
			return ERR_OpenFile;*/

		sprintf(path,"/home/huangsu/datablock/%d",1+i);
		int fd = open(path, O_RDONLY | O_SYNC, 0755);
		res = read_vec_from_file(fd, invec);
		
		close(fd);

		//TRACE(res);
		decode_direct(&invec1, outvec ,(const BIT **)inv_submatrix);
		//decode(&invec1, outvec,submatrix);	
		
		
		for(size_t j =0; j < outvec->v_size; j++)
		{
			if(f_info->f_bytes >= writed +  outvec->d_size)
			{
				writed += outvec->d_size;
				
				write(fd1 ,outvec->base[j] , outvec->d_size );
				lseek(fd1,0,SEEK_END);
				//fsync(fd1);
			}else if( f_info->f_bytes > writed ){
				//TRACE(j);
				write(fd1 ,outvec->base[j] , f_info->f_bytes -writed );
				//lseek(fd1,0,SEEK_END);
				//fsync(fd1);
				writed = f_info->f_bytes;
			}
			
		}
		fsync(fd1);
		
	}
	delete []invec1.base;
	delete []submatrix;
	
	close(fd1);
	destory_vec(invec);
	destory_vec(outvec);
	return  writed;
}




//辅助函数
 struct VEC* alloc_vec(size_t v, size_t d)
{
	if(v<=0 || d<=0)
		return NULL;
	
	struct VEC * vec = new struct VEC;
	vec->base = new BYTE* [v];
	for(size_t i=0 ; i<v;++i)
	{
		vec->base[i] = new BYTE [d];
	}
	vec->v_size = v;
	vec->d_size = d;
	return vec;
}
 void destory_vec(struct VEC* vec)
{
	if(!vec)
		return ;
	
	for(size_t i=0 ; i<vec->v_size;++i)
	{
		if(vec->base[i])
			delete [] vec->base[i];
	}
	if(vec->base)
		delete [] vec->base;
	delete vec;
}













