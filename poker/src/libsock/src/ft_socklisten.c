#include "ft_libsock.h"

t_bool		ft_socklisten(t_socket *sock)
{
	if (sock->sock != -1 && sock->server && sock->max_queue > 0)
	{
		if (listen(sock->sock, sock->max_queue) != -1)
			return (true);
	}
	return (false);
}
