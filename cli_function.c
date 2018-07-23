#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "function.h"

#define MAXLINE 1024

void str_cli(FILE *fp, int sockfd)
{
	char sendline[MAXLINE], recvline[MAXLINE];

	//从指定的流(标准输入)读取一行存入sendline中。
	while (fgets(sendline, MAXLINE, fp) != NULL)//定义于stdio.h头文件中，调用成功返回sendline首地址，到达文件尾端或出错返回NULL
	{
		if (writen(sockfd, sendline, strlen(sendline)) < 0)
			return;

		if (readline(sockfd, recvline, MAXLINE) == 0)
		{
			printf("str_cli: server terminated prematurely");
			return;
		}
		fputs(recvline, stdout);
	}
}

ssize_t writen(int fd, const void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = vptr;
	nleft = n;

	while (nleft > 0)
	{
		if ((nwritten = write(fd, ptr, nleft)) <= 0)
		{
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
			{
				perror("write error: ");
				return -1;
			}
		}

		nleft -= nwritten;
		ptr += nwritten;
	}
	return n;
}

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
	ssize_t n, rc;
	char c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; ++n)
	{
again:
		if ((rc = read(fd, &c, 1)) == 1)
		{
			*ptr++ = c;
			if (c == '\n')
				break;
		}
		else if (rc == 0)
		{
			*ptr = 0;
			return n - 1;
		}
		else
		{
			if (errno == EINTR)
				goto again;
			return -1;
		}
	}

	*ptr = 0;
	return n;
}
