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

	if(argc > 3)
	{
		printf("Too many input arguments. Excess arguments will be ignored.\n");
	}

	void (*copy_fun)(int, int) = &copy_rw;
	
	while((c = getopt(argc, argv, "mh")) != -1)
	{
		switch(c)
		{
			case 'h':
			h_flag = 1;
			break
		}
	printf("EOPSY\n");
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
