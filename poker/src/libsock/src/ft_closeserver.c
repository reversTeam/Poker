#include "ft_libsock.h"

void			ft_closeserver(t_socket_server *server)
{
	t_socket	*cur;
	t_socket	*tmp;

	cur = server->begin_sock;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		if (tmp->data)
			tmp->destroy_data(tmp->data);
		ft_sockdestroy(tmp);
	}
	free(server);
}
