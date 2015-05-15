#include "ft_common.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static size_t	count_str(char *str, char c)
{
	char	*ptr;
	size_t	count;

	while (*str && *str == c)
		++str;
	count = 0;
	while ((ptr = strchr(str, c)))
	{
		if (ptr - str != 0)
			++count;
		while (*ptr && *ptr == c)
			++ptr;
		str = ptr;
	}
	if (!ptr && strlen(str))
		++count;
	return (count);
}

char			**ft_strsplit(char *str, char c)
{
	char			**tab;
	const size_t	count = count_str(str, c);
	size_t			i;
	char			*ptr;

	if (!(tab = (char **)malloc(sizeof(char *) * (count + 1))))
		return (NULL);
	i = 0;
	while (i < count && str)
	{
		while (str && *str && *str == c)
			++str;
		ptr = strchr(str, c);
		if (ptr && ptr - str)
			tab[i] = strndup(str, ptr - str);
		else if (!ptr && strlen(str) != 0)
			tab[i] = strdup(str);
		str = ptr;
		++i;
	}
	tab[i] = 0;
	return (tab);
}
