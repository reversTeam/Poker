#include "ft_libsock.h"

t_socket	*ft_sockcreate(t_bool server)
{
	t_socket	*sock;

	if (!server)
		sock = malloc(sizeof(t_socket));
	else
		sock = malloc(sizeof(t_socket_server));
	if (!sock)
		return (NULL);
	if (!server)
		bzero(sock, sizeof(t_socket));
	else
		bzero(sock, sizeof(t_socket_server));
	sock->server = server;
	sock->sock = -1;
	return (sock);
}
