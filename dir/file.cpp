
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFFER_SIZE 1024

int main(int argc, char **argv)
{
	int form_fd, to_fd;
	long file_len = 0;
	int ret = 1;
	char buffer[BUFFER_SIZE];
	char *ptr;
	if (argc != 3)
	{
		printf("Usage:%s formfile tofile\n", argv[1]);
		exit(1);
	}
	if ((form_fd = open(argv[1], O_RDONLY|O_CREAT)) == -1)
	{
		printf("Open %s Error\n", argv[1]);
		exit(1);
	}

	if ((to_fd = open(argv[2], O_WRONLY|O_CREAT)) == -1)
	{
		printf("Open %s Error\n", argv[2]);
		exit(1);
	}
	unsigned long filesize = -1;
	struct stat statbuff;
	if(stat(argv[1], &statbuff) < 0)
	{
		printf("stat Error\n");
	}
	else
	{
		printf("form file size is %ld\n", statbuff.st_size);
	}
	file_len = lseek(form_fd, 0L, SEEK_END);
	lseek(form_fd, 0L, SEEK_SET);
	printf("form file size is %d\n", file_len);

	while (ret)
	{
		printf("%d\n", file_len);
		ret = read(form_fd, buffer, BUFFER_SIZE);
		if (ret == -1)
		{
			printf("read Error\n");
			exit(1);
		}
		write(to_fd, buffer, ret);
		file_len -= ret;
		bzero(buffer, BUFFER_SIZE);
	}
	printf("there are %d byte(s) data left without copy\n", file_len);
	close(form_fd);
	close(to_fd);
	exit(0);
}
