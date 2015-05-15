#include "ft_libsock.h"

static void		ft_doerror(t_socket_server *sock, t_socket *cur, t_bool dis)
{
	if (cur->prev)
		cur->prev->next = cur->next;
	else
		sock->begin_sock = cur->next;
	if (cur->next)
		cur->next->prev = cur->prev;
	else
		sock->end_sock = cur->prev;
	if (!dis && sock->error_callback)
		sock->error_callback(cur);
	if (dis)
		ft_sockdestroy(cur);
}

static void	init(t_socket_server *sock)
{
	t_socket	*cur;
	t_socket	*tmp;

	FD_ZERO(&sock->read_fd);
	FD_ZERO(&sock->write_fd);
	FD_ZERO(&sock->error_fd);
	sock->max_fd = sock->sock.sock;
	FD_SET(sock->sock.sock, &sock->read_fd);
	cur = sock->begin_sock;
	while (cur)
	{
		tmp = cur->next;
		if (!cur->disconnect)
		{
			if (cur->read_wait && sock->read_callback)
				FD_SET(cur->sock, &sock->read_fd);
			if (cur->write_wait && sock->write_callback)
				FD_SET(cur->sock, &sock->write_fd);
			FD_SET(cur->sock, &sock->error_fd);
			if (cur->sock > sock->max_fd)
				sock->max_fd = cur->sock;
		}
		else
			ft_doerror(sock, cur, true);
		cur = tmp;
	}
}

static t_bool	ft_newclient(t_socket_server *sock)
{
	t_socket	*cl;

	if (!(cl = ft_sockaccept((t_socket *)sock)))
		return (false);
	cl->read_wait = true;
	cl->next = NULL;
	if (sock->end_sock)
	{
		sock->end_sock->next = cl;
		cl->prev = sock->end_sock;
		sock->end_sock = cl;
	}
	else
	{
		sock->begin_sock = cl;
		sock->end_sock = cl;
	}
	if (sock->new_client)
		sock->new_client(cl);
	return (true);
}

static void		ft_doclients(t_socket_server *sock)
{
	t_socket	*cur;
	t_socket	*next;
	static char	buffer[1024];

	cur = sock->begin_sock;
	while (cur)
	{
		next = cur->next;
		if (FD_ISSET(cur->sock, &sock->read_fd))
		{
			if (!ft_sockread(cur, buffer, 1024))
				ft_doerror(sock, cur, false);
			else
				sock->read_callback(cur, buffer, strlen(buffer));
		}
		else if (FD_ISSET(cur->sock, &sock->write_fd) && sock->write_callback)
			sock->write_callback(cur);
		else if (FD_ISSET(cur->sock, &sock->error_fd))
			ft_doerror(sock, cur, false);
		cur = next;
	}
}

void		ft_sockserver(t_socket_server *sock)
{
	int		n;

	if (ft_socklistenserver(&sock->sock))
	{
		while (42)
		{
			init(sock);
			n = select(sock->max_fd + 1, &sock->read_fd, &sock->write_fd,\
					&sock->error_fd, NULL);
			if (n > 0)
			{
				if (FD_ISSET(sock->sock.sock, &sock->read_fd))
				{
					if (ft_newclient(sock))
						continue ;
				}
				else
				{
					ft_doclients(sock);
					continue ;
				}
			}
			break ;
		}
	}
}
