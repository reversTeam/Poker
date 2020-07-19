#include "ft_common.h"
#include <stdlib.h>

char		*ft_strtolower(char *s)
{
	int		i;
	char	*str;

	i = 0;
	str = ft_memalloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!str)
		exit(1);
	while (s[i])
	{
		if (s[i] >= 'A' && s[i] <= 'Z')
			str[i] = s[i] + 32;
		else
			str[i] = s[i];
		++i;
	}
	str[i] = 0;
	return (str);
}
