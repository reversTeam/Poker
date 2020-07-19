#include "ft_server.h"
#include <fcntl.h>
#include "ft_libsock.h"

void	new_client(t_socket *sock)
{
	t_user	*user;

	user = malloc(sizeof(t_user));
	bzero(user, sizeof(t_user));
	sock->data = user;
	user->sock = sock;
}

void	error_client(t_socket *sock)
{
	printf("error client\n");
	ft_sockdestroy(sock);
}

void	write_client(t_socket *sock)
{
	t_user	*user;
	char	*find;

	user = sock->data;
	sock->write_wait = false;
	(void)sock;
}

void	read_client(t_socket *sock, char *buffer, size_t len)
{
	t_user	*user;

	user = sock->data;
	if (user->read_buffer)
		user->read_buffer = ft_strnjoin(user->read_buffer, strlen(user->read_buffer), buffer, len);
	else
		user->read_buffer = strndup(buffer, len);
	if (strchr(user->read_buffer, '\n'))
	{
		dispatch_msg(user);
		if (user->illegal_msg)
			user->sock->disconnect = true;
	}
}

int		main(void)
{
	t_socket_server	*sock;

	sock = (t_socket_server *)ft_sockcreate(true);
	sock->sock.port = 1234;
	sock->sock.max_queue = 20;
	sock->read_callback = read_client;
	sock->write_callback = write_client;
	sock->error_callback = error_client;
	sock->new_client = new_client;
	ft_sockserver(sock);
	return (0);
}
