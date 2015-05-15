#include "ft_server.h"

static void		print_hand(t_table *table, t_hand *hand)
{
	info(table, "Coeur : ");
	info(table, ft_itoa(hand->color[HEART / 2].nb));
	info(table, "\n");
	info(table, "Pique : ");
	info(table, ft_itoa(hand->color[SPADE / 2].nb));
	info(table, "\n");
	info(table, "Trefle : ");
	info(table, ft_itoa(hand->color[CLUB / 2 - 1].nb));
	info(table, "\n");
	info(table, "Carreau : ");
	info(table, ft_itoa(hand->color[DIAMOND / 2].nb));
	info(table, "\n");

	info(table, "2 : ");
	info(table, ft_itoa(hand->value[TWO - TWO].nb));
	info(table, "\n");
	info(table, "3 : ");
	info(table, ft_itoa(hand->value[THREE - TWO].nb));
	info(table, "\n");
	info(table, "4 : ");
	info(table, ft_itoa(hand->value[FOUR - TWO].nb));
	info(table, "\n");
	info(table, "5 : ");
	info(table, ft_itoa(hand->value[FIVE - TWO].nb));
	info(table, "\n");
	info(table, "6 : ");
	info(table, ft_itoa(hand->value[SIX - TWO].nb));
	info(table, "\n");
	info(table, "7 : ");
	info(table, ft_itoa(hand->value[SEVEN - TWO].nb));
	info(table, "\n");
	info(table, "8 : ");
	info(table, ft_itoa(hand->value[EIGHT - TWO].nb));
	info(table, "\n");
	info(table, "9 : ");
	info(table, ft_itoa(hand->value[NINE - TWO].nb));
	info(table, "\n");
	info(table, "10 : ");
	info(table, ft_itoa(hand->value[TEN - TWO].nb));
	info(table, "\n");
	info(table, "Valet : ");
	info(table, ft_itoa(hand->value[JACK - TWO].nb));
	info(table, "\n");
	info(table, "Dame : ");
	info(table, ft_itoa(hand->value[QUEEN - TWO].nb));
	info(table, "\n");
	info(table, "Roi : ");
	info(table, ft_itoa(hand->value[KING - TWO].nb));
	info(table, "\n");
	info(table, "As : ");
	info(table, ft_itoa(hand->value[AS - TWO].nb));
	info(table, "\n");
}

static void		debug_player_hand(t_player *player, char *hand_name)
{
	int		i;
	char	*value;

	i = 0;
	info(player->table, "---------------------\n");
	info(player->table, player->login);
	info(player->table, " : ");
	info(player->table, hand_name);
	info(player->table, "---------------------\n");
	while (i < 5)
	{
		value = get_value_name(get_value(player->hand[i]));
		info(player->table, value);
		info(player->table, get_color_name(get_color(player->hand[i])));
		info(player->table, " ");
		free(value);
		++i;
	}
	info(player->table, "\n---------------------\n");
}

static void		debug_deck(t_table *table, t_deck *deck)
{
	int		i;
	char	*value;

	i  = 0;
	while (i < (int)deck->nb_card)
	{
		value = get_value_name(get_value(deck->card[i]));
		info(table, value);
		info(table, get_color_name(get_color(deck->card[i])));
		info(table, " ");
		free(value);
		++i;
	}
	info(table, "\n");
}

static void			sort_card_by_max_value(t_deck *deck)
{
	int			i;
	t_card		tmp;

	i = 0;
	while (++i < (int)deck->nb_card)
	{
		if (deck->card[i] > deck->card[i - 1])
		{
			tmp = deck->card[i];
			deck->card[i] = deck->card[i - 1];
			deck->card[i - 1] = tmp;
			i = 0;
		}
	}
}

static void		add_card_in_hand(t_hand *hand, t_card card)
{
	int		color;
	int		value;

	color = get_color(card) / 2;
	color = color == 4 ? 3 : color;
	value = get_value(card) - TWO;
	hand->color[color].card[hand->color[color].nb] = card;
	hand->color[color].nb++;
	hand->value[value].card[hand->value[value].nb] = card;
	hand->value[value].nb++;
}

static t_hand	*create_hand(t_table *table, t_player *player)
{
	int		max_card_player;
	int		flop_size;
	int		i;
	t_hand	*hand;

	flop_size = g_mode[table->game->mode].flop_size;
	max_card_player = g_mode[table->game->mode].max_card_player;
	hand = malloc(sizeof(t_hand));
	bzero(hand, sizeof(t_hand));
	hand->all = create_void_deck(table, flop_size + max_card_player);
	i = 0;
	while (i < flop_size)
	{
		add_card_to_deck(hand->all, table->flop->card[i]);
		i++;
	}
	i = 0;
	while (i < max_card_player)
	{
		add_card_to_deck(hand->all, player->card[i]);
		i++;
	}

	/*
	add_card_to_deck(hand->all, create_card(HEART, FOUR, hand->all));
	add_card_to_deck(hand->all, create_card(HEART, FIVE, hand->all));
	add_card_to_deck(hand->all, create_card(SPADE, QUEEN, hand->all));
	add_card_to_deck(hand->all, create_card(HEART, THREE, hand->all));
	add_card_to_deck(hand->all, create_card(HEART, TEN, hand->all));
	add_card_to_deck(hand->all, create_card(SPADE, AS, hand->all));
	add_card_to_deck(hand->all, create_card(SPADE, JACK, hand->all));
	*/
	sort_card_by_max_value(hand->all);
	debug_deck(table, hand->all);
	i = -1;
	while (++i < (int)hand->all->nb_card)
		add_card_in_hand(hand, hand->all->card[i]);
	return (hand);
}

static int		has_quinte_flush(t_player *player, t_hand *hand)
{
	int			i;
	int			j;
	int			n;

	i = 0;
	while (i < 4)
	{
		if (hand->color[i].nb > 4)
		{
			j = 0;
			n = 0;
			while (j < hand->color[i].nb)
			{
				if (n == 0 || get_value(hand->color[i].card[j]) == get_value(hand->color[i].card[j -1]) - 1)
				{
					player->hand[n] = hand->color[i].card[j];
					++n;
					if (j == hand->color[i].nb - 1 && get_value(hand->color[i].card[j]) == TWO
						&& get_value(hand->color[i].card[0]) == AS && n == 4)
					{
						player->hand[4] = hand->color[i].card[0];
						++n;
					}
				}
				else
				{
					n = 0;
					--j;
				}
				if (n == 5)
				{
					debug_player_hand(player, "QUINTE_FLUSH\n");
					player->hand_value = QUINTE_FLUSH;
					return (0);
				}
				++j;
			}
		}
		++i;
	}
	return (1);
}

static int		has_quads(t_player *player, t_hand *hand)
{
	int			i;
	int			j;
	t_bool		quads;
	t_bool		simple;

	i = 13;
	quads = false;
	simple = false;
	while (--i >= 0)
	{
		if (hand->value[i].nb == 4)
		{
			j = 0;
			quads = true;
			while (j < 4)
			{
				player->hand[j] = hand->value[i].card[j];
				++j;
			}
		}
		else if (!simple && hand->value[i].nb != 0)
		{
			player->hand[4] = hand->value[i].card[0];
			simple = true;
		}
		if (simple && quads)
		{
			debug_player_hand(player, "QUADS\n");
			player->hand_value = QUADS;
			return (0);
		}
	}
	return (1);
}

static int		has_full(t_player *player, t_hand *hand)
{
	int			i;
	int			j;
	t_bool		brelan;
	t_bool		paire;

	i = 13;
	paire = false;
	brelan = false;
	while (--i >= 0)
	{
		if (hand->value[i].nb == 3)
		{
			brelan = true;
			j = 0;
			while (j < 3)
			{
				player->hand[j] = hand->value[i].card[j];
				++j;
			}
		}
		else if (hand->value[i].nb == 2)
		{
			paire = true;
			j = 0;
			while (j < 2)
			{
				player->hand[j + 3] = hand->value[i].card[j];
				++j;
			}
		}
		if (paire && brelan)
		{
			player->hand_value = FULL;
			debug_player_hand(player, "FULL\n");
			return (0);
		}
	}
	return (1);
}

static int		has_color(t_player *player, t_hand *hand)
{
	int		i;
	int		j;

	i = 0;
	while (i < 4)
	{
		if (hand->color[i].nb >= 5)
		{
			j = 0;
			player->hand_value = COLOR;
			while (j < 5)
			{
				player->hand[j] = hand->color[i].card[j];
				++j;
			}
			debug_player_hand(player, "COLOR\n");
			return (0);
		}
		++i;
	}
	return (1);
}

static int		has_quinte(t_player *player, t_hand *hand)
{
	int		i;
	int		n;
	int		old;

	n = 0;
	i = 13;
	old = 13;
	while (--i >= 0)
	{
		if (hand->value[i].nb > 0 && (n == 0 || old - 1 == i))
		{
			player->hand[n] = hand->value[i].card[0];
			old = i;
			++n;
			if (i == 0 && n == 4 && hand->value[0].nb > 0 && hand->value[12].nb > 0)
			{
				player->hand[4] = hand->value[12].card[0];
				++n;
			}
		}
		else
			n = 0;
		if (n == 5)
		{
			player->hand_value = QUINTE;
			debug_player_hand(player, "QUINTE\n");
			return (0);
		}
	}
	return (1);
}

static int		has_brelan(t_player *player, t_hand *hand)
{
	int			i;
	int			j;
	int			n;
	t_bool		brelan;

	i = 13;
	n = 0;
	brelan = false;
	while (--i >= 0)
	{
		if (hand->value[i].nb == 3)
		{
			j = 0;
			brelan = true;
			while (j < 3)
			{
				player->hand[j] = hand->value[i].card[j];
				++j;
			}
		}
		else if (n < 2 && hand->value[i].nb > 0)
		{
			player->hand[n + 3] = hand->value[i].card[0];
			++n;
		}
		if (n == 2 && brelan)
		{
			player->hand_value = BRELAN;
			debug_player_hand(player, "BRELAN\n");
			return (0);
		}
	}
	return (1);
}

static int		has_two_paire(t_player *player, t_hand *hand)
{
	int			i;
	int			j;
	int			n;
	t_bool		simple;

	i = 13;
	n = 0;
	simple = false;
	while (--i >= 0)
	{
		if (hand->value[i].nb == 2)
		{
			j = 0;
			while (j < 2)
			{
				player->hand[(n * 2) + j] = hand->value[i].card[j];
				++j;
			}
			++n;
		}
		else if (!simple && hand->value[i].nb > 0)
		{
			simple = true;
			player->hand[4] = hand->value[i].card[0];
		}
		if (n == 2 && simple)
		{
			player->hand_value = TWO_PAIR;
			debug_player_hand(player, "TWO PAIR\n");
			return (0);
		}
	}
	return (1);
}

static int		has_paire(t_player *player, t_hand *hand)
{
	int			i;
	int			j;
	int			n;
	t_bool		paire;

	i = 13;
	n = 0;
	paire = false;
	while (--i >= 0)
	{
		if (hand->value[i].nb == 2)
		{
			j = 0;
			paire = true;
			while (j < 2)
			{
				player->hand[j] = hand->value[i].card[j];
				++j;
			}
		}
		else if (n < 3 && hand->value[i].nb > 0)
		{
			player->hand[n + 2] = hand->value[i].card[0];
			++n;
		}
		if (n == 3 && paire)
		{
			player->hand_value = ONE_PAIR;
			debug_player_hand(player, "ONE PAIRE\n");
			return (0);
		}
	}
	return (1);
}

static int		hight_card(t_player *player, t_hand *hand)
{
	int		i;
	int		n;

	i = 13;
	n = 0;
	while (--i >= 0)
	{
		if (hand->value[i].nb > 0)
		{
			player->hand[n] = hand->value[i].card[0];
			++n;
		}
		if (n == 5)
		{
			player->hand_value = HIGHT_CARD;
			debug_player_hand(player, "HIGHT CARD\n");
			return (0);
		}
	}
	return (1);
}

void			show_down(t_table *table)
{
	t_hand	*hand;
	int		max_player_table;
	int		i;

	i = 0;
	max_player_table = g_mode[table->game->mode].max_player_table;
	while (i < max_player_table)
	{
		if (table->player[i] && table->player[i]->user
			&& table->player[i]->role != SPECTATOR
			&& table->player[i]->status != TO_WAIT_PLAY
			&& table->player[i]->status != TO_FOLD)
		{
			hand = create_hand(table, table->player[i]);
			if (has_quinte_flush(table->player[i], hand))
			{
				if (has_quads(table->player[i], hand))
				{
					if (has_full(table->player[i], hand))
					{
						if (has_color(table->player[i], hand))
						{
							if (has_quinte(table->player[i], hand))
							{
								if (has_brelan(table->player[i], hand))
								{
									if (has_two_paire(table->player[i], hand))
									{
										if (has_paire(table->player[i], hand))
											hight_card(table->player[i], hand);
									}
								}
							}
						}
					}
				}
			}
			display_table(table);
			destroy_deck(hand->all);
			free(hand);
		}
		++i;
	}
}
