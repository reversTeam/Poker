#include "ft_libsock.h"

t_bool		ft_sockreadline(t_socket *sock, char **line)
{
	if (sock->sock != -1)
	{
		if (ft_getnextline(sock->sock, line) > 0)
			return (true);
	}
	return (false);
}
