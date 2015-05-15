#include "ft_common.h"

char	*ft_strncpy(char *dest, char *src, size_t n)
{
	return ((char *)ft_memcpy((void *)dest, (const void *)src, n));
}
