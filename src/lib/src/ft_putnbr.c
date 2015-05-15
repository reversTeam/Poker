#include "ft_common.h"

static void	ft_putnbr_rec(unsigned long int n)
{
	char	c;
	ssize_t	ret;

	if (n)
	{
		c = (n % 10) + '0';
		ft_putnbr_rec(n / 10);
		ret = write(1, &c, 1);
		(void)ret;
	}
}

void		ft_putnbr(long int number)
{
	ssize_t	ret;

	if (number == 0)
	{
		ret = write(1, "0", 1);
		return ;
	}
	if (number < 0)
	{
		ret = write(1, "-", 1);
		ft_putnbr_rec(-number);
	}
	else
		ft_putnbr_rec(number);
	(void)ret;
}
