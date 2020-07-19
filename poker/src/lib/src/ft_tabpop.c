#include "ft_common.h"
#include <stdlib.h>

t_bool	ft_tabpop(char ***tab)
{
	char	**new;
	int		i;

	i = 0;
	if (*tab)
		i = ft_tablen(*tab);
	else
		return (true);
	if (i > 0 || !(new = malloc(sizeof(char *) * (i - 1))))
		return (false);
	i = 0;
	while (*tab && (*tab)[i] && (*tab)[i + 1])
	{
		new[i] = (*tab)[i + 1];
		++i;
	}
	new[i] = 0;
	if (*tab)
	{
		free((*tab)[0]);
		free(*tab);
	}
	*tab = new;
	return (true);
}
