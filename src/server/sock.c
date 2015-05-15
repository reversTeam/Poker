#include "ft_server.h"
#include <sys/ioctl.h>
#include "ft_libsock.h"
#include <signal.h>
#include <locale.h>

void	new_client(t_socket *sock)
{
	t_user	*user;

	user = malloc(sizeof(t_user));
	bzero(user, sizeof(t_user));
	sock->data = user;
	sock->destroy_data = disconnect_user;
	user->sock = sock;
	user->selector = NO_SELECTOR;
	user->game = NO_GAME;
	user->mode = NULL;
	user->table = NULL;
}

void	error_client(t_socket *sock)
{
	disconnect_user(sock->data);
	ft_sockdestroy(sock);
}

void	add_data_to_write(t_user *user, char *msg)
{
	size_t	len = strlen(msg);
	char	*tmp;

	if (user->write_buffer)
	{
		tmp = user->write_buffer;
		user->write_buffer = ft_strnjoin(user->write_buffer, strlen(user->write_buffer), msg, len);
		free(tmp);
	}
	else
		user->write_buffer = strdup(msg);
	if (strchr(user->write_buffer, '\n'))
		user->sock->write_wait = true;
}

void	write_client(t_socket *sock)
{
	t_user	*user;
	char	*find;
	char	*tmp;

	user = sock->data;
	if ((find = strchr(user->write_buffer, '\n')))
	{
		tmp = user->write_buffer;
		*find = 0;
		ft_sockwriteline(sock, user->write_buffer, false);
		user->write_buffer = strdup(find + 1);
		free(tmp);
		find = strchr(user->write_buffer, '\n');
	}
	if (!find)
		sock->write_wait = false;
	(void)sock;
}

void	read_client(t_socket *sock, char *buffer, size_t len)
{
	t_user	*user;
	char	*tmp;

	user = sock->data;
	if (user->read_buffer)
	{
		tmp = user->read_buffer;
		user->read_buffer = ft_strnjoin(user->read_buffer, strlen(user->read_buffer), buffer, len);
		free(tmp);
	}
	else
		user->read_buffer = strndup(buffer, len);
	if (strchr(user->read_buffer, '\n'))
	{
		dispatch_msg(user);
		if (user->illegal_msg)
		{
			user->sock->disconnect = true;
			disconnect_user(user);
		}
	}
}

void		disconnect_user(void *data)
{
	t_table		*table;
	t_user		*user;

	user = (t_user*)data;
	if (user)
	{
		if (user->player && user->player->table)
		{
			table = user->player->table;
			remove_player_in_table(user->player->table, user->player);
			display_table(table);
		}
		if (user->read_buffer)
			free(user->read_buffer);
		if (user->write_buffer)
			free(user->write_buffer);
		if (user->login)
			free(user->login);
		free(user);
	}
}

void		broadcast_all_table(t_table *table, char *msg)
{
	int			i;
	int			max_player_table;

	max_player_table = g_mode[table->game->mode].max_player_table;
	i = 0;
	while (i < max_player_table)
	{
		if (table->player[i] && table->player[i]->user)
			add_data_to_write(table->player[i]->user, msg);
		++i;
	}
}

void		broadcast_table(t_user *user, char *msg)
{
	int			i;
	t_table		*table;
	int			max_player_table;

	table = user->player->table;
	max_player_table = g_mode[table->game->mode].max_player_table;
	i = 0;
	while (i < max_player_table)
	{
		if (table->player[i] && table->player[i] != user->player)
			add_data_to_write(table->player[i]->user, msg);
		++i;
	}
}

void		init_server(char *port)
{
	g_server = (t_socket_server *)ft_sockcreate(true);
	if (port)
		g_server->sock.port = atoi(port);
	else
		g_server->sock.port = 1234;
	g_server->sock.max_queue = 20;
	g_server->read_callback = read_client;
	g_server->write_callback = write_client;
	g_server->error_callback = error_client;
	g_server->new_client = new_client;
	ft_sockserver(g_server);
}
