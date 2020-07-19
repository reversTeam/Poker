#include "ft_server.h"

static t_bool	is_end_round(t_table *table)
{
	int			i;
	int			all;
	int			all_in;
	int			fold;
	int			other;
	int			max_player_table;

	i = 0;
	all = 0;
	all_in = 0;
	fold = 0;
	other = 0;
	max_player_table = g_mode[table->game->mode].max_player_table;
	while (i < max_player_table)
	{
		if (table->player[i] && table->player[i]->role != SPECTATOR
			&& table->player[i]->status != TO_WAIT_PLAY)
		{
			if (table->player[i]->status == TO_FOLD)
				++fold;
			else if (table->player[i]->status == TO_ALL_IN)
			{
				other++;
				++all_in;
			}
			else if (table->player[i]->status != TO_WAIT && table->player[i]->cash == table->cash
					&& (int) table->flop->nb_card == g_mode[table->game->mode].flop_size)
				++other;
			++all;
		}
		++i;
	}
	if (all == all_in || all - 1 == fold || other == all || all_in + fold == all || all == 1 || other + all_in + fold == all)
	{
//		if (all - fold > 1)
//		{
			end_flop(table);
			show_down(table);
//		}
		return (true);
	}
	else
		return (false);
}

void		change_player_active(t_table *table, t_player *player)
{
	int			i;
	int			max_player_table;

	if (table->start_round)
	{
		i = table->active + 1;
		max_player_table = g_mode[table->game->mode].max_player_table;
		if (is_end_round(table))
		{
			end_round(table);
			display_table(table);
			return ;
		}
		if (player == NULL || table->player[table->active] == player)
		{
			while (i != table->active)
			{
				if (table->player[i] && table->player[i]->role != SPECTATOR
					&& table->player[i]->status != TO_WAIT_PLAY
					&& table->player[i]->status != TO_FOLD && table->player[i]->status != TO_ALL_IN)
					break;
				if (++i == max_player_table)
					i = 0;
			}
			if ((table->player[i]->cash < table->cash && table->player[i]->role == TO_ALL_IN)
				|| (table->player[i]->cash == table->cash && table->player[i]->status != TO_WAIT)
			)
			{
				table->active = table->start;
				if (!table->flop->nb_card)
					flop_open(table);
				else
					flop_add_card(table);
			}
			else
				table->active = i;
			add_data_to_write(table->player[table->active]->user, "PLEASE TO PLAY\n");
		}
	}
	display_table(table);
}

void			action_to_call(t_user *user, char **args)
{
	(void)args;
	if (user->player
		&& user->player->table
		&& user->player->table->start_round
		&& user->player->table->cash > 0
		&& user->player->table->cash > user->player->cash
		&& user->player->role != SPECTATOR
		&& user->player->status != TO_ALL_IN
		&& user->player->status != TO_FOLD
		&& user->player == user->player->table->player[user->player->table->active]
	)
	{
		player_to_call(user->player);
		return ;
	}
	user->illegal_msg = true;
}

void			action_to_raise(t_user *user, char **args)
{
	t_cash		mise;

	if (user->player
		&& args[0]
		&& user->player->table
		&& user->player->table->start_round
		&& user->player->role != SPECTATOR
		&& user->player->status != TO_ALL_IN
		&& user->player->status != TO_FOLD
		&& user->player == user->player->table->player[user->player->table->active]
	)
	{
		mise = (t_cash) atoi(args[0]);
		if (mise >= user->player->table->big_blind
			|| (user->player->sold - (user->player->table->cash - user->player->cash) <= user->player->table->big_blind
				&& user->player->sold - (user->player->table->cash - user->player->cash) == mise)
		)
		{
			player_to_raise(user->player, mise);
			return ;
		}
	}
	user->illegal_msg = true;
}

void			action_to_fold(t_user *user, char **args)
{
	(void)args;
	if (user->player
		&& user->player->table
		&& user->player->table->start_round
		&& user->player->role != SPECTATOR
		&& user->player->status != TO_ALL_IN
		&& user->player->status != TO_FOLD
		&& user->player == user->player->table->player[user->player->table->active]
	)
	{
		player_to_fold(user->player);
		return ;
	}
	user->illegal_msg = true;
}

void			action_to_check(t_user *user, char **args)
{
	(void)args;
	if (user->player
		&& user->player->table
		&& user->player->table->start_round
		&& user->player->role != SPECTATOR
		&& user->player->status != TO_ALL_IN
		&& user->player->status != TO_FOLD
		&& user->player == user->player->table->player[user->player->table->active]
		&& user->player->cash == user->player->table->cash
	)
	{
		player_to_check(user->player);
		return ;
	}
	user->illegal_msg = true;
}

void			action_to_all_in(t_user *user, char **args)
{
	(void)args;
	if (user->player
		&& user->player->table
		&& user->player->table->start_round
		&& user->player->role != SPECTATOR
		&& user->player->status != TO_ALL_IN
		&& user->player->status != TO_FOLD
		&& user->player == user->player->table->player[user->player->table->active]
	)
	{
		player_to_all_in(user->player);
		return ;
	}
	user->illegal_msg = true;
}
