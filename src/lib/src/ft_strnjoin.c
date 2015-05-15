#include "ft_common.h"
#include <stdlib.h>

char	*ft_strnjoin(const char *s1, size_t len1, const char *s2, size_t len2)
{
	char			*str;

	str = (char *)malloc(len1 + len2 + 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, s1, len1);
	ft_memcpy(str + len1, s2, len2);
	str[len1 + len2] = 0;
	return (str);
}
