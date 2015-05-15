#include "ft_common.h"

void	ft_putendl_fd(char *str, int fd)
{
	ssize_t	ret;

	ret = write(fd, str, ft_strlen(str));
	ret = write(fd, "\n", 1);
	(void)ret;
}
