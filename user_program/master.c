#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#define PAGE_SIZE 4096
#define BUF_SIZE 512

/**/
#define MAP_SIZE (PAGE_SIZE * 10)

size_t get_file_size(const char* filename); // get the size of the input file

/**/
int min(int a, int b) 
{ 
	if(a < b)
		return a;
	else
		return b;
}


int main (int argc, char* argv[])
{
	char buf[BUF_SIZE];
	int i, j, dev_fd, file_fd; // the fd for the device and the fd for the input file
	size_t ret, file_size, offset = 0, tmp;
	char file_name[50], method[20];
	char *kernel_address = NULL, *file_address = NULL;
	struct timeval start; // time struct
	struct timeval end; // timr struct
	double trans_time; //calulate the time between the device is opened and it is closed

	// -------------------------------------------------------------------------------

	void *file_mem_mapped, *kernel_mem_mapped;
	int len;

	struct timeval exclude_wating;

	// -------------------------------------------------------------------------------

	// get "file name" , "fcntl or mmap method" .
	// master side don't need IP ?
	/* 2 arguments */
	strcpy(file_name, argv[1]);
	strcpy(method, argv[2]);

	/* open master device */
	if( (dev_fd = open("/dev/master_device", O_RDWR)) < 0)
	{
		perror("failed to open /dev/master_device\n");
		return 1;
	}
	gettimeofday(&start ,NULL);

	/* open file */
	if( (file_fd = open(file_name, O_RDWR)) < 0 )
	{
		perror("failed to open input file\n");
		return 1;
	}

	/* calculate file size in bits ? */
	if( (file_size = get_file_size(file_name)) < 0)
	{
		perror("failed to get filesize\n");
		return 1;
	}


	if(ioctl(dev_fd, 0x12345677) == -1) // 0x12345677 : create socket and accept the connection from the slave
	{
		perror("ioclt server create socket error\n");
		return 1;
	}

	/* waiting above */


	// debug
	gettimeofday(&exclude_wating ,NULL);

	switch(method[0]) // (argument) input method
	{
		case 'f': // fcntl : read()/write()
			do
			{
				ret = read(file_fd, buf, sizeof(buf)); // read from the input file
				write(dev_fd, buf, ret);//write to the the device
			}while(ret > 0);
			break;
		// -------------------------------------------------------------------------------
		case 'm': // mmap 
			for (i = 0; i < file_size; i += MAP_SIZE)
			{
				len = min(MAP_SIZE, file_size - i);

				file_mem_mapped = mmap(NULL, len, PROT_READ, MAP_SHARED, file_fd, i);
				kernel_mem_mapped = mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, dev_fd, i);

				memcpy(kernel_mem_mapped, file_mem_mapped, len); // copy from file_mem_mapped to kernel_mem_mapped
				ioctl(dev_fd, 0x12345678, len); // master_IOCTL_MMAP == 0x12345678
			}
			


			ioctl(dev_fd, 0x111, kernel_mem_mapped);

			break;
		// -------------------------------------------------------------------------------
	}

	if(ioctl(dev_fd, 0x12345679) == -1) // end sending data, close the connection ,master_IOCTL_EXIT == 0x12345679
	{
		perror("ioclt server exits error\n");
		return 1;
	}

	gettimeofday(&end, NULL);
	trans_time = (end.tv_sec - start.tv_sec)*1000 + (end.tv_usec - start.tv_usec)*0.0001;
	printf("Master: Transmission time: %lf ms, File size: %ld bytes\n", trans_time, file_size / 8);
	// debug
	printf("Master take off waiting time: \nTransmission time: %lf ms, File size: %ld bytes\n", (end.tv_sec - exclude_wating.tv_sec)*1000 + (end.tv_usec - exclude_wating.tv_usec)*0.0001, file_size / 8);

	close(file_fd);
	close(dev_fd);

	return 0;
}

size_t get_file_size(const char* filename)
{
    struct stat st;

	/* Get file attributes for FILE and put them in BUF.  */
    stat(filename, &st);
    return st.st_size;
}
