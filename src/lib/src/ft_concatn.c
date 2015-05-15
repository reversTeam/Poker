#include "ft_common.h"
#include <stdlib.h>
#include <stdarg.h>

char	*ft_concatn(t_bool freefirst, size_t n, ...)
{
	va_list	va;
	size_t	i;
	char	*str;
	char	*tmp;

	va_start(va, n);
	i = 0;
	str = NULL;
	while (i < n)
	{
		if (str)
		{
			tmp = str;
			str = ft_strjoin(str, va_arg(va, char *));
			if (freefirst)
				free(tmp);
			freefirst = true;
		}
		else
			str = va_arg(va, char *);
		++i;
	}
	return (str);
}
