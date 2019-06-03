#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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
				return 1;
			default:
				fprintf(stderr, "getopt error\n");
				return 2;
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
			return 3;
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
			return 3;
		}
		in_file = argv[1];
		out_file = argv[2];
		copy_fun = &copy_rw;
	}

	printf("In: %s out: %s\n", in_file, out_file);
	(*copy_fun)(0,0);
		
	return 0;
}

void help()
{
	printf("Help Info\n");
}

void copy_rw(int fd_from, int fd_to)
{
	printf("RW copy\n");
}

void copy_mm(int fd_from, int fd_to)
{
	printf("MM copy\n");
}
