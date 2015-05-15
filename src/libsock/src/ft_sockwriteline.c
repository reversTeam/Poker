#include "ft_libsock.h"

t_bool		ft_sockwriteline(t_socket *sock, char *line, t_bool crlf)
{
	const size_t	len = strlen(line);

	if (sock->sock != -1)
	{
		if (ft_ssockwrite(sock, line, len))
		{
			if (crlf && ft_ssockwrite(sock, "\13\n", 2))
				return (true);
			else if (!crlf && ft_ssockwrite(sock, "\n", 1))
				return (true);
		}
	}
	return (false);
}
