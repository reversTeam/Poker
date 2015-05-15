#include "ft_server.h"

int					get_color(t_card c)
{
	if (c & SPADE)
		return (SPADE);
	if (c & CLUB)
		return (CLUB);
	if (c & HEART)
		return (HEART);
	return (DIAMOND);
}

int					get_value(t_card c)
{
	return (c >> 4);
}

t_card				create_card(t_card color, t_card value, t_deck *deck)
{
	t_card			c;

	if (value > g_mode[deck->table->game->mode].card_end)
		return (0);
	if (color > 8)
		return (0);
	c = value;
	c = c << 4;
	c |= color;
	return (c);
}

static t_card		remove_card(t_deck *from, unsigned int index)
{
	t_card			c;

	if (index >= from->nb_card)
		return (0);
	c = from->card[index];
	while (index < from->nb_card - 1)
	{
		from->card[index] = from->card[index + 1];
		index++;
	}
	from->card[index] = 0;
	from->nb_card--;
	return (c);
}

void				add_card_to_deck(t_deck *to, t_card c)
{
	to->card[to->nb_card] = c;
	to->nb_card++;
}

t_card				get_random_card(t_deck *from, t_deck *to)
{
	unsigned int	rand;
	t_card			c;

	if (read(g_randfd, &rand, 4) <= 0)
		return (0);
	rand = rand % from->nb_card;
	if (!(c = remove_card(from, rand)))
		return (0);
	if (to != NULL)
		add_card_to_deck(to, c);
	return (c);
}

void			recover_player_card(t_player *player)
{
	int		i;
	int		max_card_player;

	if (player && player->table)
	{
		i = 0;
		max_card_player = g_mode[player->table->game->mode].max_card_player;
		while (i < max_card_player)
		{
			if (player->card[i])
			{
				add_card_to_deck(player->table->burned, player->card[i]);
				player->card[i] = NO_CARD;
			}
			++i;
		}
	}
}

void			recover_players_card(t_table *table)
{
	int		i;
	int		j;
	int		max_player_table;
	int		max_card_player;

	i = 0;
	max_player_table = g_mode[table->game->mode].max_player_table;
	max_card_player = g_mode[table->game->mode].max_card_player;
	while (i < max_player_table)
	{
		if (table->player[i])
		{
			j = 0;
			while (j < max_card_player)
			{
				if (table->player[i]->card[j] != NO_CARD)
				{
					add_card_to_deck(table->deck, table->player[i]->card[j]);
					table->player[i]->card[j] = NO_CARD;
				}
				++j;
			}
		}
		++i;
	}
}

void				recover_flop(t_table *table)
{
	int		flop_size;
	int		i;

	i = 0;
	flop_size = g_mode[table->game->mode].flop_size;
	while (i < flop_size)
	{
		if (table->flop->card[i])
		{
			add_card_to_deck(table->deck, table->flop->card[i]);
			table->flop->card[i] = NO_CARD;
			table->flop->nb_card--;
		}
		++i;
	}
}

void				recover_burned(t_table *table)
{
	int		i;
	int		burned_size;

	i = 0;
	burned_size = g_mode[table->game->mode].burned_size;
	while (i < burned_size)
	{
		if (table->burned->card[i])
		{
			add_card_to_deck(table->deck, table->burned->card[i]);
			table->burned->card[i] = NO_CARD;
			table->burned->nb_card--;
		}
		++i;
	}
}
