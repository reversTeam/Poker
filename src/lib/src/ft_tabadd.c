#include "ft_common.h"
#include <stdlib.h>

t_bool		ft_tabadd(char ***tab, char *str)
{
	char	**new;
	int		i;

	i = 0;
	if (*tab)
		i = ft_tablen(*tab);
	if (!(new = malloc(sizeof(char *) * (i + 2))))
		return (false);
	i = 0;
	while (*tab && (*tab)[i])
	{
		new[i] = (*tab)[i];
		++i;
	}
	new[i] = str;
	new[i + 1] = 0;
	if (*tab)
		free(*tab);
	*tab = new;
	return (true);
}
