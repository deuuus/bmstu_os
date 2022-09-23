#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define OK 0
#define FILE_NAME "data/alphabet.txt"

int main(void)
{
	int fd1 = open(FILE_NAME, O_RDONLY);
	int fd2 = open(FILE_NAME, O_RDONLY);
	int rc1, rc2 = 1;
	
	char c;
	
	while (rc1 == 1 || rc2 == 1)
	{
		if ((rc1 = read(fd1, &c, 1)) == 1)
			write(1, &c, 1);
		
		if ((rc2 = read(fd2, &c, 1)) == 1)
			write(1, &c, 1);
	}
	
	fprintf(stdout, "\n");
	
	return OK;
}