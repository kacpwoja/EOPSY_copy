#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Size of a chunk used in copying with read()/write(), in bytes
// 0 means filesize
#define RW_CHUNK_SIZE 0

// Printing help info
void help();
// Copying functions
void copy_rw(int fd_from, int fd_to);
void copy_mm(int fd_from, int fd_to);

int main(int argc, char** argv)
{
	// No arguments given
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
	
	// Getting options
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

	// Memory Map mode
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
		// Choose filenames and function
		in_file = argv[2];
		out_file = argv[3];
		copy_fun = &copy_mm;
	}
	// Read-Write mode
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
		// Choose filenames and function
		in_file = argv[1];
		out_file = argv[2];
		copy_fun = &copy_rw;
	}

	// Opening the files
	in_fd = open(in_file, O_RDONLY);
	if(in_fd == -1)
	{
		perror("Input file open error\n");
		exit(1);
	}
	// Getting input file access rights to copy them
	struct stat buf;
	if(fstat(in_fd, &buf) == -1)
	{
		perror("fstat error\n");
		exit(1);
	}

	// Opening the output file
	out_fd = open(out_file, O_RDWR | O_CREAT, buf.st_mode);
	if(in_fd == -1)
	{
		perror("Output file open error\n");
		exit(1);
	}

	// Copying
	(*copy_fun)(in_fd, out_fd);

	// Closing file descriptors
	close(in_fd);
	close(out_fd);
		
	return 0;
}

void help()
{
	printf("Program copy.\n");
	printf("Copies a file to another file.\n");
	printf("Correct usage:\n\n");
	printf("\tcopy [-m] <file_name> <new_file_name>\n");
	printf("\tcopy [-h]\n\n");
	printf("Option -m enables memory mapping, when it is not specified the program uses reading and writing.\n");
	printf("<file_name> is the name of the file to be copied\n");
	printf("<new_file_name> is the target file.\n");
	printf("Running with -h or no arguments shows this help.\n");
}

void copy_rw(int fd_from, int fd_to)
{
	size_t chunk;

	// If chunk size = 0, load the size of input file and set it as chunk size
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

	// Buffer to move data through
	char* buffer = (char*) malloc(chunk*sizeof(char));
	ssize_t result;
	
	// Read data from input and write to output until EOF
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

	free(buffer);
	
	return;
}

void copy_mm(int fd_from, int fd_to)
{
	// Get size of input file
	struct stat buf;
	if(fstat(fd_from, &buf) == -1)
	{
		perror("fstat error\n");
		exit(1);
	}

	// Pointers to memory maps
	char* buffer_in;
	char* buffer_out;

	buffer_in = mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, fd_from, 0);
	if(buffer_in == (void*) -1)
	{
		perror("mmap in error\n");
		exit(1);
	}
	
	// Changing output file size to input file size before mapping
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

	// Copy
	buffer_out = memcpy(buffer_out, buffer_in, buf.st_size);
	if(buffer_out == (void*) -1)
	{
		perror("mcpy error\n");
		exit(1);
	}

	// Unmapping (unnecessary, but just in case)
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

	return;
}
