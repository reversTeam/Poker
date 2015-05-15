#include "ft_common.h"

void	ft_putnstr_fd(char *str, int fd, size_t n)
{
	ssize_t	ret;

	ret = write(fd, str, n);
	(void)ret;
}
