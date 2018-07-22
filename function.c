#include <stdio.h>
#include <unistd.h>
#include "function.h"

#define MAXLINE 1024

void str_echo(int sockfd)
{
	ssize_t n;
	char buf[MAXLINE];

again_read:
	while ((n = read(sockfd, buf, MAXLINE)) > 0)
		if (writen(sockfd, buf, n) < 0)
			return;

	if (n < 0 && errno == EINTR)
		goto again_read;
	else if (n < 0)
	{
		perror("read error: ");
		return;
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
