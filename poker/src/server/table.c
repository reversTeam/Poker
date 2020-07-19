#include "ft_server.h"

t_table			*create_table(t_game *game, int small_blind, int big_blind)
{
	t_table		*table;
	t_deck		*deck;
	int			i;

	table = (t_table*) malloc(sizeof(t_table));
	if (!table)
		return (NULL);
	i = 0;
	table->player = (t_player**)malloc(sizeof(t_player*) * (g_mode[game->mode].max_player_table));
	while (i < g_mode[game->mode].max_player_table)
	{
		table->player[i] = NULL;
		++i;
	}
	table->game = game;
	table->nb_player = 0;
	table->nb_spectator = 0;
	table->nb_wait = 0;
	table->small_blind = small_blind;
	table->big_blind = big_blind;
	table->dealer = -1;
	table->active = -1;
	table->burned = create_void_deck(table, g_mode[table->game->mode].burned_size);
	table->flop = create_void_deck(table, g_mode[table->game->mode].flop_size);
	table->cash = 0;
	table->next = NULL;
	table->prev = NULL;
	table->active = -1;
	table->start_round = false;
	table->display = init_display(game);
	deck = create_deck(table);
	if (!deck)
	{
		free(table);
		return (NULL);
	}
	table->deck = deck;
	return (table);
}

int		add_player_in_table(t_table *table, t_user *user)
{
	t_player	*player;
	int			i;

	i = 0;
	while (i < g_mode[table->game->mode].max_player_table)
	{
		if (!table->player[i])
		{
			player = create_player(table, user);
			if (!player)
				return (-1);
			table->player[i] = player;
			table->nb_spectator++;
			return (0);
		}
		++i;
	}
	return (1);
}

int		remove_player_in_table(t_table *table, t_player *player)
{
	int			i;

	i = 0;
	while (i < g_mode[table->game->mode].max_player_table)
	{
		if (table->player[i] == player)
		{
			table->player[i] = NULL;
			remove_player(player);
			return (0);
		}
		++i;
	}
	return (1);
}

int		distribute_card(t_table *table)
{
	int		i;
	int		j;

	i = 0;
	while (i < g_mode[table->game->mode].max_card_player)
	{
		j = 0;
		while (j < g_mode[table->game->mode].max_player_table)
		{
			if (table->player[j] && table->player[j]->role != SPECTATOR && table->player[j]->status != TO_WAIT_PLAY)
				add_random_card_player(table->player[j], table->deck);
			++j;
		}
		++i;
	}
	return (0);
}

int		change_dealer(t_table *table)
{
	int		dealer;
	int		small;
	int		big;

	if (table->dealer >= 0)
	{
		dealer = get_next_player(table, table->dealer);
		if (table->player[table->dealer] && table->player[table->dealer]->role != SPECTATOR)
			table->player[table->dealer]->role = PLAYER;
	}
	else
		dealer = 0;
	if (dealer >= 0)
	{
		small = get_next_player(table, dealer);
		if (small >= 0)
		{
			big = get_next_player(table, small);
			if (big >= 0)
			{
				table->active = get_next_player(table, big);
				table->start = get_next_player(table, big);
				table->player[dealer]->role = DEALER;
				table->player[dealer]->blind = NO_BLIND;
				table->player[small]->blind = SMALL_BLIND;
				table->player[big]->blind = BIG_BLIND;
				table->dealer = dealer;
				return (0);
			}
		}
	}
	return (-1);
}

int		flop_open(t_table *table)
{
	int		i;
	int		j;
	char	*value;
	char	*color;

	i = 0;
	if ((int)table->flop->nb_card < g_mode[table->game->mode].flop_open)
	{
		clean_status_for_next_card(table);
		while (i < g_mode[table->game->mode].flop_open)
		{
			get_random_card(table->deck, table->flop);
			j = 0;
			while (j < g_mode[table->game->mode].max_player_table)
			{
				if (table->player[j])
				{
					value = get_value_name(get_value(table->flop->card[i]));
					color = get_color_name(get_color(table->flop->card[i]));
					add_data_to_write(table->player[j]->user, "FLOP ");
					add_data_to_write(table->player[j]->user, color);
					add_data_to_write(table->player[j]->user, " ");
					add_data_to_write(table->player[j]->user, value);
					add_data_to_write(table->player[j]->user, "\n");
					free(value);
				}
				++j;
			}
			++i;
		}
		return (0);
	}
	else
		return (-1);
}

int		flop_add_card(t_table *table)
{
	int			i;
	int			nb_card;
	char		*value;
	char		*color;

	nb_card = (int) table->flop->nb_card;
	i = 0;
	if (nb_card < g_mode[table->game->mode].flop_size)
	{
		clean_status_for_next_card(table);
		get_random_card(table->deck, table->burned);
		get_random_card(table->deck, table->flop);
		while (i < g_mode[table->game->mode].max_player_table)
		{
			if (table->player[i] && table->flop->card[nb_card])
			{
				value = get_value_name(get_value(table->flop->card[nb_card]));
				color = get_color_name(get_color(table->flop->card[nb_card]));
				add_data_to_write(table->player[i]->user, "FLOP ");
				add_data_to_write(table->player[i]->user, color);
				add_data_to_write(table->player[i]->user, " ");
				add_data_to_write(table->player[i]->user, value);
				add_data_to_write(table->player[i]->user, "\n");
				free(value);
			}
			++i;
		}
		return (0);
	}
	else
		return (-1);
}

void			destroy_table(t_table *table)
{
	int		max_player_table;
	int		i;

	max_player_table = g_mode[table->game->mode].max_player_table;
	i = 0;
	while (i < max_player_table)
	{
		if (table->player[i])
			remove_player(table->player[i]);
		++i;
	}
	free(table->player);
	destroy_deck(table->deck);
	destroy_deck(table->burned);
	destroy_deck(table->deck);
	destroy_display(table);
}
