#include "ft_common.h"
#include <stdlib.h>

char	*ft_strndup(const char *s, size_t n)
{
	char	*str;

	str = malloc(n + 1);
	if (str)
	{
		ft_memcpy(str, s, n);
		str[n] = 0;
	}
	return (str);
}
