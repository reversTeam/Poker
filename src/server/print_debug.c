#include "ft_server.h"

char			*get_color_name(int c)
{
	if (c == SPADE)
		return ("♠");
	else if (c == HEART)
		return ("♥");
	else if (c == DIAMOND)
		return ("♦");
	else if (c == CLUB)
		return ("♣");
	else
		return (NULL);
}

char			*get_value_name(int c)
{
	if (c == AS)
		return (strdup("A"));
	else if (c < JACK)
		return (ft_itoa(c));
	else if (c == JACK)
		return (strdup("V"));
	else if (c == QUEEN)
		return (strdup("Q"));
	else if (c == KING)
		return (strdup("K"));
	else
		return (NULL);
}

void		print_card(t_card c)
{
	char	*color;
	char	*value;

	if (c)
	{
		color = get_color_name(get_color(c));
		value = get_value_name(get_value(c));
		printf("        + \033[0;47m\033[1;30m %2s %s \033[0;0m\n", value, color);
		if (value)
			free(value);
	}
	else
		printf("        - Slot de carte vide\n");
}

void		print_player(t_player *player)
{
	int		i;

	i = 0;
	if (player)
	{
		if (player->role == SPECTATOR)
			printf("        ☺ ");
		else if (player->role == DEALER)
			printf("        € ");
		else
			printf("        • ");
		if (player->blind == SMALL_BLIND)
			printf("■ ");
		else if (player->blind == BIG_BLIND)
			printf("█ ");
		else
			printf("  ");
		if (player->role != SPECTATOR)
			printf("%-15s %5lld / %-5lld", player->login, player->cash, player->sold);
		else
			printf("%-15s [ SPECTATOR ]", player->login);
		if (player->role != SPECTATOR)
		{
			if (player->status == TO_WAIT)
				printf("[ WAIT ]");
			else if (player->status == TO_CALL)
				printf("[ CALL ]");
			else if (player->status == TO_RAISE)
				printf("[ RAISE ]");
			else if (player->status == TO_CHECK)
				printf("[ CHECK ]");
			else if (player->status == TO_FOLD)
				printf("[ FOLD ]");
			else if (player->status == TO_ALL_IN)
				printf("[ ALL IN ]");
		}
		printf("\n");
		if (player->role != SPECTATOR)
		{
			while (i < g_mode[player->table->game->mode].max_card_player)
			{
				printf("    ");
				print_card(player->card[i]);
				++i;
			}
		}
	}
	else
		printf("        - Aucun player\n");
}

void		print_table(t_table *table)
{
	int		i;

	i = 0;
	printf("\n    I. LISTE DES PLAYERS\n");
	while (i < g_mode[table->game->mode].max_player_table)
	{
		print_player(table->player[i]);
		++i;
	}
	printf("\n        - Petite Blinde : %lld\n", table->small_blind);
	printf("        - Grosse Blinde : %lld\n", table->big_blind);
	printf("        - Coup en cours : %lld\n", table->cash);
	printf("\n    II. CARTE DANS LE DECK\n");
	print_deck(table->deck);
	printf("\n");
	printf("\n    III. CARTE DANS LE FLOP\n");
	print_flop(table->flop);
	printf("\n");
	printf("\n    IV. CARTE BRULEES\n");
	print_burned(table->burned);
	printf("\n");
}

void	print_game(t_game *game)
{
	t_table		*cur;
	int			i;

	cur = game->table_start;
	i = 1;
	while (cur)
	{
		printf("#################################################\n");
		printf("#        INFORMATION SUR LA TABLE Nº %d\n", i);
		printf("#################################################\n");
		print_table(cur);
		cur = cur->next;
		++i;
	}
}

void	print_deck(t_deck *deck)
{
	short			i;

	i = 0;
	while (i < g_mode[deck->table->game->mode].deck_size)
	{
		print_card(deck->card[i]);
		++i;
	}
	printf("        .: Deck card size: %u / %d :.\n", deck->nb_card, g_mode[deck->table->game->mode].deck_size);
}

void	print_flop(t_deck *flop)
{
	short			i;

	i = 0;
	while(i < g_mode[flop->table->game->mode].flop_size)
	{
		print_card(flop->card[i]);
		++i;
	}
	printf("        .: Flop card size: %u / %d :.\n", flop->nb_card, g_mode[flop->table->game->mode].flop_size);
}

void	print_burned(t_deck *burned)
{
	short			i;

i = 0;
	while(i < g_mode[burned->table->game->mode].burned_size)
	{
		print_card(burned->card[i]);
		++i;
	}
		printf("        .: Burned card size: %u / %d :.\n", burned->nb_card, g_mode[burned->table->game->mode].burned_size);
}
