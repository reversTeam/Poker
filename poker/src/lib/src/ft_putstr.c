#include "ft_common.h"

void	ft_putstr(char *str)
{
	ssize_t	ret;

	ret = write(1, str, ft_strlen(str));
	(void)ret;
}
