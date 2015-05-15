#include "ft_common.h"

void	ft_putnendl(char *str, size_t n)
{
	ssize_t	ret;

	ret = write(1, str, n);
	ret = write(1, "\n", 1);
	(void)ret;
}
