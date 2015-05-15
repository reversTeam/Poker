#include "ft_server.h"

t_game			*create_game(short mode)
{
	t_game		*game;

	game = (t_game*) malloc(sizeof(t_game));
	if (!game)
		return (NULL);
	game->table_start = NULL;
	game->table_end = NULL;
	game->mode = mode;
	return (game);
}

int				add_table_in_game(t_game *game, int small_blind, int big_blind)
{
	t_table		*table;

	table = create_table(game, small_blind, big_blind);
	if (!table)
		return (-1);
	if (game->table_end)
	{
		game->table_end->next = table;
		table->prev = game->table_end;
	}
	else
	{
		game->table_start = table;
		game->table_end = table;
	}
	game->table_end = table;
	return (0);
}

int				remove_table_in_game(t_table *table)
{
	t_table		*cur;
	t_game		*game;
	int			i;
	int			max_player_table;

	game = table->game;
	cur = game->table_start;
	max_player_table = g_mode[game->mode].max_player_table;
	while (cur)
	{
		if (cur == table)
		{
			if (cur->prev)
				cur->prev->next = cur->next;
			else
				game->table_start = cur->next;
			if (cur->next)
				cur->next->prev = cur->prev;
			else
				game->table_end = cur->prev;
			if (cur->player)
			{
				i = 0;
				while (i < max_player_table)
				{
					if (cur->player[i])
						remove_player_in_table(cur, cur->player[i]);
					++i;
				}
				free(cur->player);
			}
			destroy_deck(cur->burned);
			destroy_deck(cur->flop);
			destroy_deck(cur->deck);
			destroy_display(cur);
			free(cur);
			return (0);
		}
		cur = cur->next;
	}
	return (1);
}

void		end_game(void)
{
	int			i;
	t_table		*cur;
	t_table		*tmp;

	i = 0;
	while (i < NB_MODE)
	{
		if (g_game[i])
		{
			cur = g_game[i]->table_start;
			while (cur)
			{
				tmp = cur;
				cur = cur->next;
				remove_table_in_game(tmp);
			}
		}
		++i;
	}
}
