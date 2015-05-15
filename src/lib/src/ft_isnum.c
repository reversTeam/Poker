#include "ft_common.h"

int		ft_isnum(int c)
{
	if ((char)c >= '0' && (char)c <= '9')
		return (1);
	return (0);
}
