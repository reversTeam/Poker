#include "ft_common.h"
#include <stdlib.h>

void	*ft_memalloc(size_t n)
{
	char	*ptr;

	ptr = (char *)malloc(n);
	if (!ptr)
		return (NULL);
	ft_memset(ptr, 0, n);
	return ((void *)ptr);
}
