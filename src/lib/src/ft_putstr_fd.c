#include "ft_common.h"

void	ft_putstr_fd(char *str, int fd)
{
	ssize_t	ret;

	ret = write(fd, str, ft_strlen(str));
	(void)ret;
}
