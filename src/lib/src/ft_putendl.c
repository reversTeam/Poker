#include "ft_common.h"

void	ft_putendl(char *str)
{
	ssize_t	ret;

	ret = write(1, str, ft_strlen(str));
	ret = write(1, "\n", 1);
	(void)ret;
}
