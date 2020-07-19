#include "ft_libsock.h"

t_bool	ft_sockread(t_socket *sock, char *buffer, size_t max)
{
	ssize_t	ret;
	int		fd;

	if ((sock && sock->sock != -1) || !sock)
	{
		fd = 0;
		if (sock)
			fd = sock->sock;
		if ((ret = read(fd, buffer, max - 1)) > 0)
		{
			buffer[ret] = 0;
			return (true);
		}
		return (ret > 0);
	}
	return (false);
}
