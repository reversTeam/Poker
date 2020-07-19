#include "ft_common.h"

int	ft_findinarray(char *array, char c)
{
	char	*s;

	if ((s = ft_strchr(array, c)))
		return (s - array);
	return (-1);
}
