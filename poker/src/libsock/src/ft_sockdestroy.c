#include "ft_libsock.h"

void	ft_sockdestroy(t_socket *sock)
{
	ft_sockclose(sock);
	free(sock);
}
