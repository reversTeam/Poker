#include "ft_common.h"
#include <stdlib.h>

void	*ft_memdup(const void *src, size_t n)
{
	void	*ptr;

	if (!(ptr = malloc(n)))
		return (NULL);
	return (ft_memcpy(ptr, src, n));
}
