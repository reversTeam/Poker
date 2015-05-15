#include <ft_common.h>
#include <stdlib.h>

char		*ft_itoa(int64_t nbr)
{
	char		neg;
	char		*str;
	long int	n;
	int			i;

	if (nbr == 0)
		return (ft_strdup("0"));
	n = 1;
	i = 0;
	neg = (nbr < 0) ? 1 : 0;
	nbr = (nbr < 0) ? nbr * -1 : nbr;
	while (n <= nbr && ++i)
		n *= 10;
	if ((str = malloc(sizeof(char) * (i + 2))) != NULL)
	{
		str[0] = '-';
		i = neg;
		while (n > 1)
		{
			str[i++] = (nbr % n / (n / 10)) + '0';
			n /= 10;
		}
		str[i] = 0;
	}
	return (str);
}
