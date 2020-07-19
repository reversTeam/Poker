#include "ft_libsock.h"

t_socket	*ft_sockaccept(t_socket *sock)
{
	int					new;
	struct sockaddr_in	sin;
	socklen_t			sin_len;
	t_socket			*newsock;

	if (sock->sock != -1 && sock->server)
	{
		sin_len = sizeof(sin);
		bzero(&sin, sin_len);
		new = accept(sock->sock, (struct sockaddr *)&sin, &sin_len);
		if (new != -1)
		{
			newsock = ft_sockcreate(false);
			if (!newsock)
				return (NULL);
			newsock->sock = new;
			newsock->host = strdup(inet_ntoa(sin.sin_addr));
			newsock->destroy_host = true;
			newsock->port = ntohs(sin.sin_port);
			return (newsock);
		}
	}
	return (NULL);
}
