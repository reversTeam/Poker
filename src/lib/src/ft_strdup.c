#include "ft_common.h"
#include <stdlib.h>
#include <stdint.h>

char	*ft_strdup(const char *s)
{
	char			*str;
	const size_t	len = ft_strlen(s);

	str = malloc(len + 1);
	return ((str) ? (char *)ft_memcpy(str, s, len + 1) : NULL);
}
