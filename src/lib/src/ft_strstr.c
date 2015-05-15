#include "ft_common.h"

char	*ft_strstr(char *str, const char *find)
{
	char	*tmp;
	size_t	i;

	while ((tmp = ft_strchr(str, find[0])))
	{
		i = 0;
		while (find[i] && find[i] == tmp[i])
			++i;
		if (!find[i])
			return (tmp);
		str = tmp + i;
	}
	return (NULL);
}
