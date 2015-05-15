#include "ft_libsock.h"

t_bool	ft_socklistenserver(t_socket *sock)
{
	if (sock->server)
	{
		if (ft_sockbind(sock, true) && ft_socklisten(sock))
			return (true);
	}
	return (false);
}
