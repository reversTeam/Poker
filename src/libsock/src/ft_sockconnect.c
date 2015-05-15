#include "ft_libsock.h"

static t_bool	error(t_socket *sock)
{
	if (sock->sock != -1)
	{
		close(sock->sock);
		sock->sock = -1;
	}
	return (false);
}

t_bool	ft_sockconnect(t_socket *sock)
{
	struct sockaddr_in	sin;
	struct hostent		*host;

	if (sock->host && sock->port && !sock->server)
	{
		sock->sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock->sock == -1)
			return (false);
		host = gethostbyname(sock->host);
		if (!host)
			return (error(sock));
		sin.sin_addr.s_addr = *(in_addr_t *)host->h_addr;
		sin.sin_port = htons(sock->port);
		sin.sin_family = AF_INET;
		if (connect(sock->sock, (struct sockaddr *)&sin, sizeof(sin)) == -1)
			return (error(sock));
		return (true);
	}
	return (false);
}
