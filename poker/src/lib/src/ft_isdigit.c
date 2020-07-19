#include "ft_common.h"

t_bool		ft_isdigit(char *str)
{
	int		i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isnum((int) str[i]))
			return (false);
		++i;
	}
	return (true);
}
