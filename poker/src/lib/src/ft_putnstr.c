#include "ft_common.h"

void	ft_putnstr(char *str, size_t n)
{
	ssize_t	ret;

	ret = write(1, str, n);
	(void)ret;
}
