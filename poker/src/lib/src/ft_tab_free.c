#include "ft_common.h"
#include <stdlib.h>

void		ft_tab_free(char **tab)
{
	int		i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		++i;
	}
	free(tab);
}
