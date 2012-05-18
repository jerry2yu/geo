#include "io.h"

struct io_file
{
	FILE *fp;
	int pos;
};

struct io_file *io_open(const char *fname) 
{
	struct io_file * const f = (io_file*)malloc(sizeof *f);
	if (NULL == f) return NULL;
	/* open file, if failed, create an empty file*/
	f->fp = fopen(fname,"rb+");
	if (NULL == f->fp) f->fp = fopen(fname,"wb+");
	if (NULL == f->fp) 
	{
		perror(fname);
		free(f);
		return NULL;
	}
	f->pos = -1;
	return f;
}

void io_close(struct io_file *f) {
	if (NULL != f) 
	{
		fclose(f->fp);
		free(f);
	}
}
/**
 * write data to file
 * @param f - file to be written
 * @param pos - writing positioin of the file
 * @param o - buffer of output content
 * @param len - output size in bytes
 */
int io_out(struct io_file *f,int pos,const void *o,int len) 
{
	if (NULL == f || -1 == pos) return -1;

	if (pos != f->pos && fseek(f->fp,pos,SEEK_SET)) 
	{
		perror("seek");
		return -1;
	}

	if (NULL != o && 1 != fwrite(o,len,1,f->fp)) 
	{
		perror("write");
		return -1;
	}

	return f->pos = pos + len;
}
/**
 * Read in data from give file
 * @param	f - input file
 * @param	pos - read position of file 
 * @param	i - input buffer to hold read data
 * @param	len - data size to read
 * @return	file position after read
 */
int io_in(struct io_file *f,int pos,void *i,int len) 
{
	if (NULL == f || -1 == pos) return -1;
	//if ( fseek(f->fp,pos,SEEK_SET) )

	if ( NULL != i )
	{
		if (pos != f->pos && fseek(f->fp,pos,SEEK_SET)) 
		{
			perror("seek");
			return -1;
		}

		if ( 1 != fread(i,len,1,f->fp) ) 
		{
			if (feof(f->fp))
				fputs("read: unexpected EOF\n",stderr); /* TODO */
			else
				perror("read");
			return -1;
		}
	}
	else
	{
		if ( fseek(f->fp,pos+len,SEEK_SET) )
		{
			perror("seek");
			return -1;
		}
	}


	return f->pos = pos + len;
}
