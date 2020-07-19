#include "ft_common.h"
#include <stdlib.h>

char	*ft_strsub(char *str, size_t start, size_t len)
{
	char	*s;

	s = (char *)malloc(len + 1);
	if (!s)
		return (NULL);
	ft_memcpy(s, str + start, len);
	s[len] = 0;
	return (s);
}
