#include "ft_server.h"

void		add_user_in_table(t_user *user, char **args)
{
	if (args[0])
	{
		add_player_in_table(g_game[HOLDEM]->table_start, user);
	}
	else
		user->illegal_msg = true;
}
