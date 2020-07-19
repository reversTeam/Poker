#include "ft_common.h"

static char	**next(char **tab)
{
	int		i;
	size_t	len;

	i = 0;
	while (tab[i])
	{
		if (tab[i][0] == '\"')
		{
			len = ft_strlen(tab[i]);
			ft_memcpy(tab[i], tab[i] + 1, len - 2);
			tab[i][len - 2] = 0;
			len = 0;
			while (tab[i][len])
			{
				if (tab[i][len] == '\n')
					tab[i][len] = ' ';
				++len;
			}
		}
		++i;
	}
	return (tab);
}

char		**ft_flagsplit(char *str)
{
	int		in_str;
	int		i;

	in_str = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '"')
			in_str = !in_str;
		else if (in_str && str[i] == ' ')
			str[i] = '\n';
		++i;
	}
	return (next(ft_strsplit(str, ' ')));
}
