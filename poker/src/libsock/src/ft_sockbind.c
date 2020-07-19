#include "ft_libsock.h"

t_bool		ft_sockbind(t_socket *sock, t_bool reuseaddr)
{
	int					val;
	const size_t		sval = sizeof(val);
	struct sockaddr_in	sin;

	bzero(&sin, sizeof(sin));
	if ((sock->sock = socket(AF_INET, SOCK_STREAM, 0)) != -1 && sock->server)
	{
		if (reuseaddr)
		{
			val = 1;
			val = setsockopt(sock->sock, SOL_SOCKET, SO_REUSEADDR, &val, sval);
			if (val == -1)
				return (false);
		}
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
		sin.sin_port = htons(sock->port);
		sin.sin_family = AF_INET;
		if (bind(sock->sock, (struct sockaddr *)&sin, sizeof(sin)) != -1)
			return (true);
	}
	return (false);
}
