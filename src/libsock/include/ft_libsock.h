#ifndef FT_LIBSOCK_H
# define FT_LIBSOCK_H

# include "ft_common.h"
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>


typedef struct s_socket			t_socket;
typedef struct s_socket_server	t_socket_server;

struct				s_socket
{
	int				sock;
	char			*host;
	t_bool			destroy_host;
	unsigned short	port;
	unsigned int	max_queue;
	t_bool			server;
	t_bool			read_wait;
	t_bool			write_wait;
	void			*data;
	t_bool			disconnect;
	t_socket		*prev;
	t_socket		*next;
	void			(*destroy_data)(void *data);
};

struct				s_socket_server
{
	t_socket		sock;
	void			(*new_client)(t_socket *sock);
	void			(*read_callback)(t_socket *sock, char *buffer, size_t len);
	void			(*write_callback)(t_socket *sock);
	void			(*error_callback)(t_socket *sock);
	fd_set			read_fd;
	fd_set			write_fd;
	fd_set			error_fd;
	int				max_fd;
	t_socket		*begin_sock;
	t_socket		*end_sock;
};

t_socket			*ft_sockcreate(t_bool server);
t_socket			*ft_sockaccept(t_socket *sock);

t_bool				ft_sockconnect(t_socket *sock);
t_bool				ft_sockclose(t_socket *sock);
t_bool				ft_socklisten(t_socket *sock);
t_bool				ft_sockbind(t_socket *sock, t_bool reuseaddr);
t_bool				ft_socklistenserver(t_socket *sock);
t_bool				ft_ssockwrite(t_socket *sock, char *buffer, size_t len);
t_bool				ft_sockwriteline(t_socket *sock, char *line, t_bool crlf);
t_bool				ft_sockread(t_socket *sock, char *buffer, size_t max);
t_bool				ft_ssockread(t_socket *sock, char *buffer, size_t len);

void				ft_sockdestroy(t_socket *sock);
void				ft_sockserver(t_socket_server *sock);
void				ft_closeserver(t_socket_server *server);

#endif
