#include "ft_server.h"

static void		clean_player(t_table *table)
{
	int		i;

	i = 0;
	while (i < g_mode[table->game->mode].max_player_table)
	{
		if (table->player[i] && table->player[i]->role != SPECTATOR)
		{
			if (table->player[i]->status == TO_WAIT_PLAY)
			{
				table->nb_wait--;
				table->nb_player++;
			}
			else if (table->player[i]->sold == 0)
			{
				player_to_fold(table->player[i]);
				table->nb_player--;
				table->nb_spectator++;
				table->player[i]->role = SPECTATOR;
			}
			table->player[i]->status = TO_WAIT;
		}
		++i;
	}
}

static void		reset_blind_round(t_table *table)
{
	int			i;

	i = 0;
	while (i < g_mode[table->game->mode].max_player_table)
	{
		
		if (table->player[i])
		{
			if (table->player[i]->status == TO_WAIT_PLAY)
			{
				table->nb_player++;
				table->nb_wait--;
			}
			table->player[i]->cash = 0;
			table->player[i]->mise = 0;
			table->player[i]->status = TO_WAIT;
			table->player[i]->blind = NO_BLIND;
		}
		++i;
	}
}

int				clean_status_for_next_card(t_table *table)
{
	int		i;

	i = 0;
	while (i < g_mode[table->game->mode].max_player_table)
	{
		if (table->player[i])
		{
			if (table->player[i]->role != SPECTATOR
				&& table->player[i]->status != TO_WAIT_PLAY
				&& table->player[i]->status != TO_ALL_IN
				&& table->player[i]->status != TO_FOLD
			)
				table->player[i]->status = TO_WAIT;
			table->player[i]->mise += table->player[i]->cash;
			table->player[i]->cash = 0;
		}
		++i;
	}
	table->cash = 0;
	table->active = table->start;
	return (0);
}

static void		send_info_game(t_table *table)
{
	int		i;
	int		max_player_table;
	char	*tmp;

	i = 0;
	max_player_table = g_mode[table->game->mode].max_player_table;
	while (i < max_player_table)
	{
		broadcast_all_table(table, "CHAIR ");
		if (table->player[i])
		{
			if (table->player[i]->role == PLAYER)
				broadcast_all_table(table, "PLAYER ");
			else if (table->player[i]->role == DEALER)
				broadcast_all_table(table, "DEALER ");
			else
				broadcast_all_table(table, "SPECTATOR ");
			broadcast_all_table(table, table->player[i]->login);
			if (table->player[i] != SPECTATOR)
			{
				broadcast_all_table(table, " ");
				tmp = ft_itoa(table->player[i]->sold);
				broadcast_all_table(table, tmp);
				if (table->player[i]->blind == SMALL_BLIND)
					broadcast_all_table(table, " SMALL_BLIND");
				else if (table->player[i]->blind == BIG_BLIND)
					broadcast_all_table(table, " BIG_BLIND");
				else if (table->active == i)
					broadcast_all_table(table, " START");
				free(tmp);
			}
		}
		else
			broadcast_all_table(table, "EMPTY");
		broadcast_all_table(table, "\n");
		++i;
	}
}

int				start_round(t_table *table)
{
	if (table->nb_player > 1)
	{
		broadcast_all_table(table, "START_HAND\n");
		table->start_round = true;
		table->cash = table->big_blind;
		change_dealer(table);
		send_info_game(table);
		get_blind(table);
		distribute_card(table);
		add_data_to_write(table->player[table->active]->user, "PLEASE TO PLAY\n");
	}
	return (0);
}

void			end_flop(t_table *table)
{
	int		flop_size;

	flop_size = g_mode[table->game->mode].flop_size;
	while ((int)table->flop->nb_card < flop_size)
	{
		if ((int)table->flop->nb_card == 0)
			flop_open(table);
		else
			flop_add_card(table);
	}
}

int				end_round(t_table *table)
{
	broadcast_all_table(table, "END_HAND\n");
	table->start_round = false;
	reset_blind_round(table);
	recover_players_card(table);
	recover_flop(table);
	recover_burned(table);
	clean_player(table);
	start_round(table);
	return (0);
}

int				get_blind(t_table *table)
{
	int				small;
	int				big;
	char			*small_blind;
	char			*big_blind;

	small = get_next_player(table, table->dealer);
	if (small >= 0)
	{
		big = get_next_player(table, small);
		if (big >= 0)
		{
			if (mise_for_player(table->player[small], table->small_blind) < 0)
				return (-1);
			if (mise_for_player(table->player[big], table->big_blind) < 0)
				return (-1);
			small_blind = ft_itoa(table->small_blind);
			big_blind = ft_itoa(table->big_blind);
			add_data_to_write(table->player[small]->user, "BLIND SMALL ");
			add_data_to_write(table->player[small]->user, small_blind);
			add_data_to_write(table->player[small]->user, "\n");
			add_data_to_write(table->player[big]->user, "BLIND BIG ");
			add_data_to_write(table->player[big]->user, big_blind);
			add_data_to_write(table->player[big]->user, "\n");
			free(small_blind);
			free(big_blind);
			return (0);
		}
	}
	return (-2);
}

int				get_next_player(t_table *table, int i)
{
	int		start;

	start = i;
	if (i < g_mode[table->game->mode].max_player_table)
	{
		while (++i != start)
		{
			if (i >= g_mode[table->game->mode].max_player_table)
				i = 0;
			if (table->player[i] && table->player[i]->role != SPECTATOR
				&& table->player[i]->status != TO_WAIT_PLAY)
				break ;
		}
		return (i);
	}
	else
		return (-1);
}

