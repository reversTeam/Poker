#include "ft_common.h"
#include <stdlib.h>

static void		ft_simple_size(char *s, int *start, int *end)
{
	int		i;
	int		len;

	i = *start;
	len = *end;
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '	' && s[len] != ' ' && s[len] != '	')
			break ;
		else
		{
			if (s[i] == ' ' || s[i] == '	' )
				++i;
			else
				--len;
		}
	}
	*start = i;
	*end = len;
}


char			*ft_trim(char *s)
{
	int		i;
	int		n;
	int		len;
	char	*str;

	i = 0;
	len = ft_strlen(s) - 1;
	ft_simple_size(s, &i, &len);

	str = malloc(sizeof(char) * (len - i + 1));
	if (!str)
		exit(1);
	n = 0;
	while (i <= len)
	{
		str[n] = s[i];
		++n;
		++i;
	}
	str[n] = 0;
	return (str);
}
