#include "ft_libsock.h"

t_bool	ft_sockclose(t_socket *sock)
{
	if (sock->sock != -1)
	{
		close(sock->sock);
		sock->sock = -1;
	}
	if (sock->host && sock->destroy_host)
	{
		free(sock->host);
		sock->host = NULL;
		sock->destroy_host = false;
		sock->port = 0;
	}
	return (true);
}
