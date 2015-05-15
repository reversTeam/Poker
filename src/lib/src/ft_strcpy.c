#include "ft_common.h"

char	*ft_strcpy(char *dest, const char *src)
{
	return ((char *)ft_memcpy((void *)dest, (const void *)src, ft_strlen(src)));
}
