#include "ft_server.h"

void		get_games(t_user *user, char **args)
{
	(void)args;
	if (!args[0] && user->game >= NO_SELECTOR)
	{
		user->selector = SELECTOR_GAME;
		add_data_to_write(user, "POKER\n");
	}
	else
		user->illegal_msg = true;
}

void		select_game(t_user *user, char **args)
{
	if (args[0] && !args[1])
	{
		if (!strcmp(args[0], "POKER") && user->selector == SELECTOR_GAME)
		{
			add_data_to_write(user, "SUCCESS SELECT GAME\n");
			user->game = GAME_POKER;
		}
		else
			user->illegal_msg = true;
	}
	else
		user->illegal_msg = true;
}

void		get_modes(t_user *user, char **args)
{
	int			i;
	int			mode;

	(void)args;
	i = 0;
	mode = 0;
	if (!args[0] && user->selector >= SELECTOR_GAME && user->game)
	{
		user->selector = SELECTOR_MODE;
		while (i < NB_MODE)
		{
			if (g_mode[i].active)
			{
				if (mode > 0)
					add_data_to_write(user, " ");
				add_data_to_write(user, g_mode[i].name);
				++mode;
			}
			++i;
		}
		add_data_to_write(user, "\n");
	}
	else
		user->illegal_msg = true;
}

void		select_mode(t_user *user, char **args)
{
	int			i;

	i = 0;
	if (args[0] && !args[1])
	{
		while (i < NB_MODE)
		{
			if (g_mode[i].active && !strcmp(args[0], g_mode[i].name))
			{
				user->mode = g_game[i];
				add_data_to_write(user, "SUCCESS SELECT MODE\n");
				return ;
			}
			++i;
		}
	}
	user->illegal_msg = true;
}

void		get_blinds(t_user *user, char ** args)
{
	t_table		*table;
	int			n;
	char		*tmp;

	(void)args;
	n = 0;
	if (!args[0] && user->selector >= SELECTOR_MODE && user->mode)
	{
		table = user->mode->table_start;
		while (table)
		{
			if (n > 0)
				add_data_to_write(user, " ");
			tmp = ft_itoa(table->small_blind);
			add_data_to_write(user, tmp);
			free(tmp);
			add_data_to_write(user, " ");
			tmp = ft_itoa(table->big_blind);
			add_data_to_write(user, tmp);
			free(tmp);
			n++;
			table = table->next;
		}
		user->selector = SELECTOR_BLIND;
		if (n > 0)
			add_data_to_write(user, "\n");
		else
			add_data_to_write(user, "NO_BLIND_DISPO\n");
	}
	else
		user->illegal_msg = true;
}

void		select_blind(t_user *user, char **args)
{
	t_table		*table;
	int			n;
	t_cash		small;
	t_cash		big;

	if (args[0] && args[1] && !args[2] && user->selector == SELECTOR_BLIND)
	{
		n = 0;
		small = atoi(args[0]);
		big = atoi(args[1]);
		if (small < big)
		{
			if ((big * 10) <= user->sold)
			{
				table = user->mode->table_start;
				while (table)
				{
					if (table->small_blind == small && table->big_blind == big)
					{
						if (!add_player_in_table(table, user))
						{
							add_data_to_write(user, "SUCCESS JOIN TABLE\n");
							display_table(table);
							return ;
						}
						++n;
					}
					table = table->next;
				}
				if (n > 0)
				{
					add_table_in_game(user->mode, small, big);
					select_blind(user, args);
					return ;
				}
			}
			else
			{
				add_data_to_write(user, "ERROR YOU DONT HAVE ENOUGH MONEY\n");
				return ;
			}
		}
	}
	user->illegal_msg = true;
}

void		create_blind(t_user *user, char **args)
{
	t_cash	small;
	t_cash	big;

	if (args[0] && args[1] && !args[2] && user->selector == SELECTOR_BLIND)
	{
		small = atoi(args[0]);
		big = atoi(args[1]);
		if ((big * 10) < user->sold)
		{
			if (small < big)
			{
				if (add_table_in_game(user->mode, small, big))
				{
					create_blind(user, args);
					return ;
				}
				else
				{
					select_blind(user, args);
					add_data_to_write(user, "SUCCESS TABLE CREATE\n");
					return ;
				}
			}
		}
		else
		{
			add_data_to_write(user, "ERROR YOU DONT HAVE ENOUGH MONEY\n");
			return ;
		}
	}
	user->illegal_msg = true;
}

void		change_role(t_user *user, char **args)
{
	t_cash			sold;
	t_cash			min;
	t_cash			max;

	if (args[0] && ((args[1] && !args[2]) || !args[1]))
	{
		if (!strcmp(args[0], "PLAYER"))
		{
			if (user->player->role == SPECTATOR)
			{
				min = (t_cash) (user->player->table->big_blind * 10);
				max = (t_cash) (min * 10);
				if (args[1])
					sold = (t_cash) atoi(args[1]);
				else
					sold = min;
				if (sold >= min && sold <= max)
				{
					user->player->role = PLAYER;
					if (!user->player->table->start_round)
					{
						user->player->table->nb_player++;
						user->player->status = TO_WAIT;
					}
					else
					{
						user->player->table->nb_wait++;
						user->player->status = TO_WAIT_PLAY;
					}
					user->player->table->nb_spectator--;
					add_data_to_write(user, "SUCCESS CHANGE ROLE PLAYER\n");
					user->player->sold = sold;
					if (user->player->table->nb_player > 1 && !user->player->table->start_round)
						start_round(user->player->table);
				}
				else
					user->illegal_msg = true;
			}
			else
				user->illegal_msg = true;
		}
		else if (!strcmp(args[0], "SPECTATOR") && !args[1])
		{
			if (user->player->role != SPECTATOR)
			{
				player_to_fold(user->player);
				user->player->role = SPECTATOR;
				add_data_to_write(user, "SUCCESS CHANGE ROLE SPECTATOR\n");
			}
			else
				user->illegal_msg = true;
		}
		else
			user->illegal_msg = true;
	}
	else
		user->illegal_msg = true;
	display_table(user->player->table);
}
