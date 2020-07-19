#include "ft_common.h"

void	ft_putnendl_fd(char *str, int fd, size_t n)
{
	ssize_t	ret;

	ret = write(fd, str, n);
	ret = write(fd, "\n", 1);
	(void)ret;
}
