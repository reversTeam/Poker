#include "ft_server.h"

t_deck		*create_deck(t_table *table)
{
	int			i;
	uint8_t		color;
	uint8_t		value;
	t_card		card;
	t_deck		*deck;

	i = 0;
	color = SPADE;
	deck = (t_deck*) malloc(sizeof(t_deck));
	if (!deck)
		return (NULL);
	deck->card = (t_card *) malloc(sizeof(t_card) * (g_mode[table->game->mode].deck_size));
	if (!deck->card)
	{
		free(deck);
		return (NULL);
	}
	deck->table = table;
	while (color <= CLUB)
	{
		value = g_mode[table->game->mode].card_start;
		while (value <= g_mode[table->game->mode].card_end)
		{
			card = create_card(color, value, deck);
			if (!card)
			{
				free(deck->card);
				free(deck);
				return (NULL);
			}
			deck->card[i] = card;
			++value;
			++i;
		}
		color *= 2;
	}
	deck->nb_card = i;
	return (deck);
}

t_deck		*create_void_deck(t_table *table, int size)
{
	t_deck		*deck;

	deck = (t_deck *) malloc(sizeof(t_deck));
	if (!deck)
		return (NULL);
	bzero(deck, sizeof(t_deck));
	deck->card = (t_card *) malloc(sizeof(t_card) * size);
	if (!deck->card)
	{
		free(deck);
		return (NULL);
	}
	bzero(deck->card, sizeof(t_card) * size);
	deck->table = table;
	deck->nb_card = 0;
	return (deck);
}

void		clean_deck(t_deck *deck)
{
	int		i;

	i = 0;
	while (i < (int)deck->nb_card)
	{
		deck->card[i] = NO_CARD;
		++i;
	}
	deck->nb_card = 0;
}

void		destroy_deck(t_deck *deck)
{
	free(deck->card);
	free(deck);
}
