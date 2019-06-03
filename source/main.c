#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// 0 means filesize
#define RW_CHUNK_SIZE 2

void help();
void copy_rw(int fd_from, int fd_to);
void copy_mm(int fd_from, int fd_to);

int main(int argc, char** argv)
{
	if(argc == 1)
	{
		help();
		return 0;
	}

	void (*copy_fun)(int, int);
	char* in_file;
	char* out_file;
	int in_fd;
	int out_fd;
	
	char m_flag = 0;
	char c;

	while((c = getopt(argc, argv, "mh")) != -1)
	{
		switch(c)
		{
			case 'h':
				help();
				return 0;
			case 'm':
				m_flag = 1;
				break;
			case '?':
				fprintf(stderr, "Unknown option '-%c'\n", optopt);
				exit(1);
			default:
				fprintf(stderr, "getopt error\n");
				exit(1);
		}
	}

	if(m_flag)
	{
		if(argc > 4)
		{
			printf("Too many input arguments. Excess arguments will be ignored.\n");
		}
		else if(argc < 4)
		{
			fprintf(stderr, "Too few input arguments.\nUse copy [-h] to see correct usage.\n");
			exit(1);
		}
		in_file = argv[2];
		out_file = argv[3];
		copy_fun = &copy_mm;
	}
	else
	{
		if(argc > 3)
		{
			printf("Too many input arguments. Excess arguments will be ignored.\n");
		}
		else if(argc < 3)
		{
			fprintf(stderr, "Too few input arguments.\nUse copy [-h] to see correct usage.\n");
			exit(1);
		}
		in_file = argv[1];
		out_file = argv[2];
		copy_fun = &copy_rw;
	}

	in_fd = open(in_file, O_RDONLY);
	if(in_fd == -1)
	{
		perror("Input file open error\n");
		exit(1);
	}
	struct stat buf;
	if(fstat(in_fd, &buf) == -1)
	{
		perror("fstat error\n");
		exit(1);
	}

	out_fd = open(out_file, O_RDWR | O_CREAT, buf.st_mode);
	if(in_fd == -1)
	{
		perror("Output file open error\n");
		exit(1);
	}

	(*copy_fun)(in_fd, out_fd);

	close(in_fd);
	close(out_fd);
		
	return 0;
}

void help()
{
	printf("Help Info\n");
}

void copy_rw(int fd_from, int fd_to)
{
	size_t chunk;

	if(!RW_CHUNK_SIZE)
	{
		struct stat buf;
		if(fstat(fd_from, &buf) == -1)
		{
			perror("fstat error\n");
			exit(1);
		}
		
		chunk = (size_t) buf.st_size;
	}
	else
	{
		chunk = RW_CHUNK_SIZE;
	}

	char buffer[chunk];
	ssize_t result;
	
	while((result = read(fd_from, buffer, chunk)) != 0)
	{
		if(result == -1)
		{
			perror("read error\n");
			exit(1);
		}
		
		if(write(fd_to, buffer, result) == -1)
		{
			perror("write error\n");
			exit(1);
		}
	}
	
	return;
}

void copy_mm(int fd_from, int fd_to)
{
	struct stat buf;
	if(fstat(fd_from, &buf) == -1)
	{
		perror("fstat error\n");
		exit(1);
	}

	char* buffer_in;
	char* buffer_out;

	buffer_in = mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, fd_from, 0);
	if(buffer_in == (void*) -1)
	{
		perror("mmap in error\n");
		exit(1);
	}
	if(ftruncate(fd_to, buf.st_size))
	{
		perror("ftruncate error\n");
		exit(1);
	}
	buffer_out = mmap(NULL, buf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_to, 0);
	if(buffer_out == (void*) -1)
	{
		perror("mmap out error\n");
		exit(1);
	}

	buffer_out = memcpy(buffer_out, buffer_in, buf.st_size);
	if(buffer_out == (void*) -1)
	{
		perror("mcpy error\n");
		exit(1);
	}

	if(munmap(buffer_in, buf.st_size) == -1)
	{
		perror("munmap in error\n");
		exit(1);
	}
	if(munmap(buffer_out, buf.st_size) == -1)
	{
		perror("munmap out error\n");
		exit(1);
	}
}
