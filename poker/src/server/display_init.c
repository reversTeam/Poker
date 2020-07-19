#include "ft_server.h"
#include <sys/ioctl.h>

static t_win	*init_win(int height, int width, int pos_y, int pos_x, size_t size)
{
	t_win		*win;

	win = malloc(size);
	if (win)
	{
		win->width = width;
		win->height = height;
		win->pos_x = pos_x;
		win->pos_y = pos_y;
		win->window = newwin(height, width, pos_y, pos_x);
		scrollok(win->window, TRUE);
		return (win);
	}
	return (NULL);
}

static void		destroy_subwin(t_subwin *subwin)
{
	delwin(subwin->window);
	free(subwin);
}

static void		destroy_display_player(t_player_win *player, int max_card_player)
{
	int		i;

	i = 0;
	if (player->card)
	{
		while (i < max_card_player)
		{
			if (player->card[i])
				destroy_subwin(player->card[i]);
			++i;
		}
		free(player->card);
	}
	delwin(player->window);
	destroy_subwin(player->name);
	destroy_subwin(player->mise);
	destroy_subwin(player->sold);
	destroy_subwin(player->statut);
	free(player);
}

static void		destroy_display_deck(t_deck_win *deck, int size)
{
	int		i;

	i = 0;
	delwin(deck->window);
	if (deck->card)
	{
		while (i < size)
		{
			if (deck->card[i])
				destroy_subwin(deck->card[i]);
			++i;
		}
		free(deck->card);
	}
	free(deck);
}

void			destroy_display(t_table *table)
{
	t_display	*display;
	int			max_player_table;
	int			max_card_player;
	int			i;
	t_mode		mode;

	display = table->display;
	mode = g_mode[table->game->mode];
	i = 0;
	max_player_table = mode.max_player_table;
	max_card_player = mode.max_card_player;
	while (i < max_player_table)
	{
		if (display->table->player[i])
			destroy_display_player(display->table->player[i], max_card_player);
		++i;
	}
	free(display->table->player);
	destroy_display_deck(display->table->burned, mode.burned_size);
	destroy_display_deck(display->table->flop, mode.flop_size);
	destroy_display_deck(display->table->deck, mode.deck_size);
	free(display->table);
	free(display->info);
	free(display->console);
	free(display);
}

void	init_colors(void)
{
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_GREEN);
	init_pair(3, COLOR_WHITE, COLOR_BLUE);
	init_pair(4, COLOR_BLACK, COLOR_WHITE);
	init_pair(5, COLOR_WHITE, COLOR_RED);
	init_pair(6, COLOR_RED, COLOR_WHITE);
	init_pair(7, COLOR_BLACK, COLOR_YELLOW);
	init_pair(8, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(9, COLOR_BLACK, COLOR_CYAN);
}

t_display	*init_display(t_game *game)
{
	struct winsize	w;
	int				i;
	t_display		*display;

	display = malloc(sizeof(t_display));
	i = 0;
	initscr();
	start_color();
	init_colors();
	curs_set(0);
	noecho();
	keypad(stdscr,TRUE);
	ioctl(0, TIOCGWINSZ, &w);
	display->height = w.ws_row;
	display->width = w.ws_col;
	display->table = (t_table_win*) init_win(display->height - CONSOLE_HEIGHT, display->width - INFO_WIDTH, 0, 0, sizeof(t_table_win));
	display->table->player = malloc(sizeof(t_player_win*) * g_mode[game->mode].max_player_table);
	display->table->deck = NULL;
	display->table->flop = NULL;
	display->table->burned = NULL;
	if (!display->table->player)
	{
		perror("Fail in the init player display\n");
		exit(1);
	}
	while (i < g_mode[game->mode].max_player_table)
	{
		display->table->player[i] = NULL;
		++i;
	}
	display->info = init_win(display->height - CONSOLE_HEIGHT, INFO_WIDTH, 0, display->width - INFO_WIDTH, sizeof(t_win));
	display->console = init_win(CONSOLE_HEIGHT, display->width, display->height - CONSOLE_HEIGHT , 0, sizeof(t_win));
	init_colors_windows(display);
	return (display);
}

void	init_colors_windows(t_display *display)
{
	bkgd(COLOR_PAIR(1));
	if (display->table && display->table->window)
	{
		wbkgd(display->table->window, COLOR_PAIR(2));
		wrefresh(display->table->window);
	}
	if (display->info && display->info->window)
	{
		wbkgd(display->info->window, COLOR_PAIR(3));
		wrefresh(display->info->window);
	}
}
