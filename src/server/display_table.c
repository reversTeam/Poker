#include "ft_server.h"

static void		display_card(t_table *table, int player_number)
{
	int				max_card_player;
	int				i;
	int				x;
	t_player_win	*player;
	char			*value;

	i = 0;
	max_card_player = g_mode[table->game->mode].max_card_player;
	x = (BOX_PLAYER_WIDTH - ((max_card_player * BOX_CARD_WIDTH) + (max_card_player - 1))) / 2;
	player = table->display->table->player[player_number];
	if (!player->card)
	{
		player->card = malloc(sizeof(t_subwin*) * max_card_player);
		bzero(player->card, sizeof(t_subwin*) * max_card_player);
	}
	while (i < max_card_player)
	{
		if (!player->card[i])
		{
			player->card[i] = malloc(sizeof(t_subwin));
			player->card[i]->parent = player->window;
			player->card[i]->width = BOX_CARD_WIDTH;
			player->card[i]->height = BOX_CARD_HEIGHT;
			player->card[i]->pos_x = player->pos_x + x;
			player->card[i]->pos_y = player->pos_y + 1;
			player->card[i]->window = subwin(
				player->window,
				player->card[i]->height,
				player->card[i]->width,
				player->card[i]->pos_y,
				player->card[i]->pos_x
			);
			table->display->table->player[player_number] = player;
		}
		wclear(player->card[i]->window);
		if (!table->player[player_number] || table->player[player_number]->role == SPECTATOR)
			wbkgd(player->card[i]->window, COLOR_PAIR(2));
		else
		{
			if (table->player[player_number]->card[i])
			{
				wbkgd(player->card[i]->window, COLOR_PAIR(4));
				value =  get_value_name(get_value(table->player[player_number]->card[i]));
				wprintw(player->card[i]->window, "%s\n", value);
				free(value);
				if (get_color(table->player[player_number]->card[i]) == HEART || get_color(table->player[player_number]->card[i]) == DIAMOND)
				{
					wattron(player->card[i]->window, COLOR_PAIR(6));
					wprintw(player->card[i]->window, "%s", get_color_name(get_color(table->player[player_number]->card[i])));
					wattroff(player->card[i]->window, COLOR_PAIR(4));
				}
				else
					wprintw(player->card[i]->window, "%s", get_color_name(get_color(table->player[player_number]->card[i])));
			}
			else
				wbkgd(player->card[i]->window, COLOR_PAIR(2));
		}
		x += 1 + BOX_CARD_WIDTH;
		++i;
	}
}

static void		display_name(t_table *table, int player_number)
{
	char			*login;
	char			*tmp;
	char			*tmp2;
	t_player_win	*player;

	player = table->display->table->player[player_number];
	if (!player->name)
	{
		player->name = malloc(sizeof(t_subwin));
		player->name->width = player->width;
		player->name->height = 1;
		player->name->pos_x = player->pos_x;
		player->name->pos_y = player->pos_y;
		player->name->parent = player->window;
		player->name->window = subwin(
			player->name->parent,
			player->name->height,
			player->name->width,
			player->name->pos_y,
			player->name->pos_x
		);
		table->display->table->player[player_number] = player;
	}
	if (table->player[player_number])
		login = strdup(table->player[player_number]->login);
	else
		login = strdup("Aucun Player");
	if (!table->player[player_number] || table->player[player_number]->role != DEALER)
	{
		
		if (!table->player[player_number] || table->player[player_number]->status != TO_WAIT_PLAY)
			wbkgd(player->name->window, COLOR_PAIR(1));
		else
			wbkgd(player->name->window, COLOR_PAIR(4));
	}
	else
		wbkgd(player->name->window, COLOR_PAIR(7));
	tmp = ft_itoa((BOX_PLAYER_WIDTH + ft_strlen(login)) / 2);
	tmp2 = ft_concatn(false, 3, "%", tmp, "s");
	wclear(player->name->window);
	wprintw(player->name->window, tmp2, login);
	free(login);
	free(tmp);
	free(tmp2);
}

static void		display_mise(t_table *table, int player_number)
{
	t_player_win		*player;

	player = table->display->table->player[player_number];
	if (!player->mise)
	{
		player->mise = malloc(sizeof(t_subwin));
		player->mise->parent = player->window;
		player->mise->width = player->width - 2;
		player->mise->height = 1;
		player->mise->pos_x = player->pos_x + 1;
		player->mise->pos_y = player->pos_y + 4;
		player->mise->window = subwin(
			player->mise->parent,
			player->mise->height,
			player->mise->width,
			player->mise->pos_y,
			player->mise->pos_x
		);
		table->display->table->player[player_number] = player;
	}
	wclear(player->mise->window);
	if (table->player[player_number])
	{
		if (table->player[player_number]->role != SPECTATOR)
		{
			wbkgd(player->mise->window, COLOR_PAIR(5));
			wprintw(player->mise->window, "%17d", table->player[player_number]->cash);
		}
		else
			wbkgd(player->mise->window, COLOR_PAIR(4));
	}
	else
		wbkgd(player->mise->window, COLOR_PAIR(5));
}

static void		display_sold(t_table *table, int player_number)
{
	t_player_win		*player;

	player = table->display->table->player[player_number];
	if (!player->sold)
	{
		player->sold = malloc(sizeof(t_subwin));
		player->sold->parent = player->window;
		player->sold->width = player->width - 2;
		player->sold->height = 1;
		player->sold->pos_x = player->pos_x + 1;
		player->sold->pos_y = player->pos_y + 5;
		player->sold->window = subwin(
			player->sold->parent,
			player->sold->height,
			player->sold->width,
			player->sold->pos_y,
			player->sold->pos_x
		);
		table->display->table->player[player_number] = player;
	}
	wclear(player->sold->window);
	if (table->player[player_number])
	{
		wbkgd(player->sold->window, COLOR_PAIR(4));
		if (table->player[player_number]->role != SPECTATOR)
		{
			wprintw(player->sold->window, "%17d", table->player[player_number]->sold);
		}
	}
	else
		wbkgd(player->sold->window, COLOR_PAIR(5));
}

static void		display_statut(t_table *table, int player_number)
{
	t_player_win		*player;

	player = table->display->table->player[player_number];
	if (!player->statut)
	{
		player->statut = malloc(sizeof(t_subwin));
		player->statut->parent = player->window;
		player->statut->width = player->width - 2;
		player->statut->height = 1;
		player->statut->pos_x = player->pos_x + 1;
		player->statut->pos_y = player->pos_y + 7;
		player->statut->window = subwin(
			player->statut->parent,
			player->statut->height,
			player->statut->width,
			player->statut->pos_y,
			player->statut->pos_x
		);
		table->display->table->player[player_number] = player;
	}
	wclear(player->statut->window);
	if (table->player[player_number])
	{
		if (table->player[player_number]->blind == SMALL_BLIND)
			wbkgd(player->statut->window, COLOR_PAIR(8));
		else if (table->player[player_number]->blind == BIG_BLIND)
			wbkgd(player->statut->window, COLOR_PAIR(9));
		else
			wbkgd(player->statut->window, COLOR_PAIR(1));
		if (table->player[player_number]->role != SPECTATOR)
		{
			if (table->player[player_number]->status == TO_CALL)
				wprintw(player->statut->window, "%12s", "CALL");
			else if (table->player[player_number]->status == TO_RAISE)
				wprintw(player->statut->window, "%12s", "RAISE");
			else if (table->player[player_number]->status == TO_CHECK)
				wprintw(player->statut->window, "%12s", "CHECK");
			else if (table->player[player_number]->status == TO_FOLD)
				wprintw(player->statut->window, "%12s", "FOLD");
			else if (table->player[player_number]->status == TO_ALL_IN)
				wprintw(player->statut->window, "%13s", "ALL IN");
			else if (table->player[player_number]->status == TO_WAIT_PLAY)
				wprintw(player->statut->window, "%13s", "NEXT GAME");
			else
				wprintw(player->statut->window, "%12s", "WAIT");
		}
		else
			wprintw(player->statut->window, "%13s", "SPECTATOR");
	}
	else
		wbkgd(player->statut->window, COLOR_PAIR(5));
}

static void		display_player(t_table *table)
{
	int				width;
	int				height;
	int				i;
	int				j;
	int				max_player_table;
	int				space_x;
	int				space_y;
	int				start_x;
	int				nb_player_width;
	t_player_win	*player;

	i = 0;
	max_player_table = g_mode[table->game->mode].max_player_table;
	width = table->display->table->width;
	height = table->display->table->height;
	nb_player_width = max_player_table - (2  * NB_PLAYER_HEIGHT);
	start_x = (2 *PADDING_TABLE) + BOX_PLAYER_WIDTH;
	space_x = (width - (2 * start_x) - (nb_player_width * BOX_PLAYER_WIDTH)) / (nb_player_width - 1);
	space_y = (height - (2 + (NB_PLAYER_HEIGHT * BOX_PLAYER_HEIGHT)) - BOX_PLAYER_HEIGHT) / (NB_PLAYER_HEIGHT - 1);
	if (!table->display->table->player)
	{
		table->display->table->player = malloc(sizeof(t_player_win*) * max_player_table);
		bzero(table->display->table->player, sizeof(t_player_win*) * max_player_table);
	}
	while (i < max_player_table)
	{
		player = table->display->table->player[i];
		if (!player)
		{
			player = malloc(sizeof(t_player_win));
			player->width = BOX_PLAYER_WIDTH;
			player->height = BOX_PLAYER_HEIGHT;
			player->parent = table->display->table->window;
			player->name = NULL;
			player->card = NULL;
			player->mise = NULL;
			player->sold = NULL;
			player->statut = NULL;
			if (i < NB_PLAYER_HEIGHT)
			{
				player->pos_x = table->display->table->pos_x + width - (BOX_PLAYER_WIDTH + PADDING_TABLE);
				player->pos_y = table->display->table->pos_y + 1 + i * space_y + i * BOX_PLAYER_HEIGHT;
			}
			else if (i >= max_player_table - NB_PLAYER_HEIGHT)
			{
				j = (i - 1) % NB_PLAYER_HEIGHT;
				player->pos_x = table->display->table->pos_x + PADDING_TABLE;
				player->pos_y = height - ((table->display->table->pos_y + 1 + j * space_y + j * BOX_PLAYER_HEIGHT) + (2 * BOX_PLAYER_HEIGHT) + (table->display->table->height % 2));
			}
			else
			{
				j = (i + 1) % nb_player_width;
				player->pos_x = width - ((table->display->table->pos_x + start_x) + (j * space_x) + (j * BOX_PLAYER_WIDTH)) - BOX_PLAYER_WIDTH;
				player->pos_y = table->display->table->pos_y + height - (BOX_PLAYER_HEIGHT + 1);
			}
			player->window = subwin(
				player->parent,
				player->height,
				player->width,
				player->pos_y,
				player->pos_x
			);
			table->display->table->player[i] = player;
		}
		wclear(player->window);
		if (table->player[i])
		{
			if (table->player[i]->role != SPECTATOR)
			{
				if (i != table->active || !table->start_round)
					wbkgd(player->window, COLOR_PAIR(3));
				else
					wbkgd(player->window, COLOR_PAIR(9));
			}
			else
				wbkgd(player->window, COLOR_PAIR(4));
		}
		else
			wbkgd(player->window, COLOR_PAIR(5));
		display_name(table, i);
		display_card(table, i);
		display_mise(table, i);
		display_sold(table, i);
		display_statut(table, i);
		++i;
	}
}

static void		display_deck(t_table *table)
{
	int			max_player_table;
	int			start;
	int			end;
	int			i;
	int			height_card;
	int			width_card;
	char		*value;
	t_subwin	*card;

	max_player_table = g_mode[table->game->mode].max_player_table;
	start = table->display->table->player[max_player_table - 1]->pos_x + table->display->table->player[max_player_table - 1]->width;
	end = table->display->table->player[0]->pos_x;
	width_card = (g_mode[table->game->mode].deck_size * 2) + (g_mode[table->game->mode].deck_size - 1);
	if (!table->display->table->deck)
	{
		table->display->table->deck = malloc(sizeof(t_deck_win));
		table->display->table->deck->card = NULL;
		table->display->table->deck->width = end - start - 2 * MARGIN_DECK;
		height_card = width_card / (table->display->table->deck->width - 3) + 1;
		height_card = (height_card * 2) + (height_card - 1) + 2;
		table->display->table->deck->height = height_card;
		table->display->table->deck->pos_x = start + MARGIN_DECK;
		table->display->table->deck->pos_y = 0;
		table->display->table->deck->parent = table->display->table->window;
		table->display->table->deck->window = subwin(
			table->display->table->window,
			table->display->table->deck->height,
			table->display->table->deck->width,
			table->display->table->deck->pos_y,
			table->display->table->deck->pos_x
		);
	}
	wbkgd(table->display->table->deck->window, COLOR_PAIR(1));
	if (!table->display->table->deck->card)
	{
		table->display->table->deck->card = malloc(sizeof(t_subwin*) * g_mode[table->game->mode].deck_size);
		bzero(table->display->table->deck->card, sizeof(t_subwin*) * g_mode[table->game->mode].deck_size);
	}
	i = 0;
	start = table->display->table->deck->pos_x;
	if (width_card < table->display->table->deck->width)
		end = (table->display->table->deck->width - width_card) / 2 - 3;
	else
		end = 0;
	wclear(table->display->table->deck->window);
	while (i < g_mode[table->game->mode].deck_size)
	{
		card = table->display->table->deck->card[i];
		if (!card)
		{
			card = malloc(sizeof(t_subwin));
			card->parent = table->display->table->deck->window;
			card->width = BOX_CARD_WIDTH;
			card->height = BOX_CARD_HEIGHT;
			end += 3;
			card->pos_x = end % (table->display->table->deck->width - 3) + start;
			card->pos_y = (end / (table->display->table->deck->width - 3)) * 3 + 1;
			card->window = subwin(
				table->display->table->deck->window,
				card->height,
				card->width,
				card->pos_y,
				card->pos_x
			);
			table->display->table->deck->card[i] = card;
		}
		wclear(card->window);
		if (!table->deck->card[i])
			wbkgd(card->window, COLOR_PAIR(2));
		else
		{
			wbkgd(card->window, COLOR_PAIR(4));
			value = get_value_name(get_value(table->deck->card[i]));
			wprintw(card->window, "%s\n", value);
			free(value);
			if (get_color(table->deck->card[i]) == HEART || get_color(table->deck->card[i]) == DIAMOND)
			{
				wattron(card->window, COLOR_PAIR(6));
				wprintw(card->window, "%s", get_color_name(get_color(table->deck->card[i])));
				wattroff(card->window, COLOR_PAIR(4));
			}
			else
				wprintw(card->window, "%s", get_color_name(get_color(table->deck->card[i])));
		}
		++i;
	}
}

static void		display_flop(t_table *table)
{
	int			flop_size;
	int			width_flop;
	int			i;
	int			color;
	int			value;
	t_deck_win	*flop;
	t_subwin	*card;
	char		*str;

	flop_size = g_mode[table->game->mode].flop_size;
	width_flop = (flop_size * 6) + ((flop_size - 1) * 2) + 4;
	flop = table->display->table->flop;
	if (!flop)
	{
		flop = malloc(sizeof(t_deck_win));
		flop->card = NULL;
		flop->parent = table->display->table->window;
		flop->width = width_flop;
		flop->height = 5 + 2;
		flop->pos_y = (table->display->table->height - flop->height) / 2;
		flop->pos_x = (table->display->table->width - flop->width) / 2;
		flop->window = subwin(
			flop->parent,
			flop->height,
			flop->width,
			flop->pos_y,
			flop->pos_x
		);
		table->display->table->flop = flop;
	}
	wbkgd(flop->window, COLOR_PAIR(3));
	if (!flop->card)
	{
		flop->card = malloc(sizeof(t_subwin*) * flop_size);
		bzero(flop->card, sizeof(t_subwin*) * flop_size);
	}
	i = 0;
	while (i < flop_size)
	{
		card = flop->card[i];
		if (!card)
		{
			card = malloc(sizeof(t_subwin));
			card->parent = flop->window;
			card->width = 6;
			card->height = 5;
			card->pos_x = (flop->pos_x + 2) + (i * 8);
			card->pos_y = flop->pos_y + 1;
			card->window = subwin(
				card->parent,
				card->height,
				card->width,
				card->pos_y,
				card->pos_x
			);
			flop->card[i] = card;
		}
		wclear(card->window);
		if (!table->flop->card[i])
			wbkgd(card->window, COLOR_PAIR(2));
		else
		{
			wbkgd(card->window, COLOR_PAIR(4));
			value = get_value(table->flop->card[i]);
			color = get_color(table->flop->card[i]);
			str = get_value_name(value);
			if (color == HEART || color == DIAMOND)
			{
				wattron(card->window, COLOR_PAIR(6));
				wprintw(card->window, "%s    %s\n", get_color_name(color), get_color_name(color));
				wattroff(card->window, COLOR_PAIR(4));
				wprintw(card->window, "  %s\n\n", str);
				wattron(card->window, COLOR_PAIR(6));
				wprintw(card->window, "%s    %s", get_color_name(color), get_color_name(color));
				wattroff(card->window, COLOR_PAIR(4));
			}
			else
			{
				wprintw(card->window, "%s    %s\n", get_color_name(color), get_color_name(color));
				wprintw(card->window, "  %s\n\n", str);
				wprintw(card->window, "%s    %s", get_color_name(color), get_color_name(color));
			}
			free(str);
		}
		++i;
	}
}

static void		display_burned(t_table *table)
{
	t_deck_win		*burned;
	t_subwin		*card;
	int				burned_size;
	int				width_burned;
	int				height_burned;
	int				i;
	int				start;
	int				color;
	int				value;
	char			*str;

	burned = table->display->table->burned;
	burned_size = g_mode[table->game->mode].burned_size;
	width_burned = (table->display->table->width - BOX_CARD_WIDTH) - (4 * PADDING_TABLE + 2 * BOX_PLAYER_WIDTH) + 2;
 	height_burned = 2 + (width_burned / (burned_size * BOX_CARD_WIDTH + (burned_size - 1))) + (table->display->height % 2);
	height_burned = height_burned < 5 ? 4 : height_burned;
	if (!burned)
	{
		burned = malloc(sizeof(t_deck_win));
		burned->parent = table->display->table->window;
		burned->width = width_burned;
		burned->height = height_burned;
		burned->card = malloc(sizeof(t_subwin*) * burned_size);
		bzero(burned->card, sizeof(t_subwin*) * burned_size);
		burned->pos_y = table->display->table->pos_y + table->display->table->height - (2 * PADDING_TABLE + BOX_PLAYER_HEIGHT);
		burned->pos_x = table->display->table->pos_x + (2 * PADDING_TABLE + BOX_PLAYER_WIDTH);
		burned->window = subwin(
			burned->parent,
			burned->height,
			burned->width,
			burned->pos_y,
			burned->pos_x
		);
		table->display->table->burned = burned;
	}
	wclear(burned->window);
	wbkgd(burned->window, COLOR_PAIR(3));
	i = 0;
	width_burned = burned_size * BOX_CARD_WIDTH + burned_size - 1 + 2;
	if (width_burned > burned->width)
		start = 0;
	else
		start = (burned->width - width_burned) / 2;
	while (i < burned_size)
	{
		card = burned->card[i];
		if (!card)
		{
			card = malloc(sizeof(t_subwin));
			card->parent = burned->window;
			card->width = BOX_CARD_WIDTH;
			card->height = BOX_CARD_HEIGHT;
			card->pos_x = start + burned->pos_x + i * BOX_CARD_WIDTH + i + 1;
			card->pos_y = burned->pos_y + 1;
			card->window = subwin(
				card->parent,
				card->height,
				card->width,
				card->pos_y,
				card->pos_x
			);
		}
		wclear(card->window);
		if (table->burned->card[i])
		{
			color = get_color(table->burned->card[i]);
			value = get_value(table->burned->card[i]);
			wbkgd(card->window, COLOR_PAIR(4));
			str = get_value_name(value);
			wprintw(card->window, "%s\n", str);
			free(str);
			if (color == HEART || color == DIAMOND)
			{
				wattron(card->window, COLOR_PAIR(6));
				wprintw(card->window, "%s", get_color_name(color));
				wattroff(card->window, COLOR_PAIR(4));
			}
			else
				wprintw(card->window, "%s", get_color_name(color));
		}
		else
			wbkgd(card->window, COLOR_PAIR(2));
		burned->card[i] = card;
		++i;
	}
}

void			display_table(t_table *table)
{
	if (table && table->display && table->display->table && table->display->table->window)
	{
		wclear(table->display->table->window);
		display_player(table);
		display_deck(table);
		display_burned(table);
		display_flop(table);
		wrefresh(table->display->table->window);

	}
}

void		info(t_table *table, char *msg)
{
	wprintw(table->display->info->window, msg);
	wrefresh(table->display->info->window);
}
