#include "ft_libsock.h"

t_bool		ft_ssockread(t_socket *sock, char *buffer, size_t len)
{
	ssize_t	ret;
	size_t	offset;

	if (sock->sock != -1)
	{
		offset = 0;
		while ((ret = read(sock->sock, buffer + offset, len - offset)) > 0)
		{
			offset += ret;
			if (offset >= len)
				break ;
		}
		return (ret != -1);
	}
	return (false);
}
