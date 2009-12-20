#include "file.h"


int file_is_existed(BYTE *file_name)
{
	return access(file_name, F_OK) == 0;
}


FILE *sys_file_open(BYTE *file_name, DWORD flags)
{
	FILE *fp = NULL;

	switch (flags) 
	{
		case MODE_READ:
			fp = fopen(file_name, "rb");
			break;
			
		case MODE_WRITE:
			fp = fopen(file_name, "wb");
			break;

		case MODE_READWRITE:
			fp = fopen(file_name, "wb+");
//			if(!fp && (flags & MODE_CREATE))
//			{
//				fp = fopen(file_name, "wb+");
//			}
			break;
		
		case MODE_APPEND:
//			file_hd = fopen(file_name, "ab+");
			fp = fopen(file_name, "ab");
			break;
			
		case MODE_UPDATE:
			fp = fopen(file_name, "rb+");
			break;
			
		case MODE_READ_T:
			fp = fopen(file_name, "r");
			break;
		case MODE_WRITE_T:
			fp = fopen(file_name, "w");
			break;
		case MODE_APPEND_T:
			fp = fopen(file_name, "a+");
			break;
		case MODE_RW_T:
			fp = fopen(file_name, "w+");
			break;
		case MODE_UPDATE_T:
			fp = fopen(file_name, "r+");
			break;
		default:
			break;
	}

	/*! error process */
	return fp;
}

void sys_file_close( FILE *fp )
{
	int fd = -1;
	if(!fp)
	{
		return;
	}
//	fd = fileno(fp);
//	fsync(fd);
	fclose(fp);

//	fp = NULL;
}

int sys_file_read(FILE *file_hd, void *buf, DWORD count)
{
	if(count == 0)
	{
		return FAILURE;
	}
	if(!file_hd)
	{
		return FAILURE;
	}
	return fread( buf, 1, count, file_hd );
}


int sys_file_write(FILE *file_hd, void *data, DWORD count)
{
	BYTE *p_data = data;
	DWORD write_cnt = 0;
	
	if(count == 0 || data == NULL || file_hd == NULL)
	{
		return FAILURE;
	}
	while(1)
	{
		write_cnt = fwrite(p_data, 1, count, file_hd);
		if(write_cnt == count)
		{
			break;
		}
		else
		{
			p_data += write_cnt;
			count -= write_cnt;
		}
	}
	
	return SUCCESS;
//	return 
}


void sys_file_save( void *buf, DWORD len, BYTE *file_name )
{
	FILE *fd = NULL;
	
	fd = sys_file_open(file_name, MODE_WRITE);
	if(fd == NULL)
	{
		perror("sys_file_open");
		return;
	}
	if(sys_file_write(fd, buf, len) != 0)
	{
		// do nothing
	}
	sys_file_close(fd);
}

