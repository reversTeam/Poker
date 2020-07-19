#include "ft_server.h"
#include <fcntl.h>

static void		init_holdem(void)
{
	t_mode		*mode;

	mode = &g_mode[HOLDEM];
	mode->name = strdup("HOLDEM");
	mode->max_player_table = 10;
	mode->max_card_player = 2;
	mode->card_start = TWO;
	mode->card_end = AS;
	mode->deck_size = 52;
	mode->flop_size = 5;
	mode->flop_open = 3;
	mode->active = true;
	mode->burned_size = (mode->max_player_table * mode->max_card_player) + mode->flop_size;
	g_game[HOLDEM] = create_game(HOLDEM);
}

static void		init_double_holdem(void)
{
	t_mode		*mode;

	mode = &g_mode[DOUBLE_HOLDEM];
	mode->name = strdup("DOUBLE_HOLDEM");
	mode->max_player_table = 10;
	mode->max_card_player = 2;
	mode->card_start = TWO;
	mode->card_end = AS;
	mode->deck_size = 52;
	mode->flop_size = 10;
	mode->flop_open = 3;
	mode->active = true;
	mode->burned_size = (mode->max_player_table * mode->max_card_player) + mode->flop_size;
	g_game[DOUBLE_HOLDEM] = create_game(DOUBLE_HOLDEM);
}

static void		init_omaha(void)
{
	t_mode		*mode;

	mode = &g_mode[OMAHA];
	mode->name = strdup("OMAHA");
	mode->max_player_table = 10;
	mode->max_card_player = 4;
	mode->card_start = TWO;
	mode->card_end = AS;
	mode->deck_size = 52;
	mode->burned_size = 39;
	mode->flop_size = 5;
	mode->flop_open = 3;
	mode->active = true;
	mode->burned_size = (mode->max_player_table * mode->max_card_player) + mode->flop_size;
	g_game[OMAHA] = create_game(OMAHA);
}

static void		init_pineapple(void)
{
	t_mode		*mode;

	mode = &g_mode[PINEAPPLE];
	mode->name = strdup("PINEAPPLE");
	mode->max_player_table = 10;
	mode->max_card_player = 3;
	mode->card_start = TWO;
	mode->card_end = AS;
	mode->deck_size = 52;
	mode->flop_size = 5;
	mode->flop_open = 3;
	mode->active = true;
	mode->burned_size = (mode->max_player_table * mode->max_card_player) + mode->flop_size;
	g_game[PINEAPPLE] = create_game(PINEAPPLE);
}

static void		init_courchevel(void)
{
	t_mode		*mode;

	mode = &g_mode[COURCHEVEL];
	mode->name = strdup("COURCHEVEL");
	mode->max_player_table = 8;
	mode->max_card_player = 5;
	mode->card_start = TWO;
	mode->card_end = AS;
	mode->deck_size = 52;
	mode->flop_size = 5;
	mode->flop_open = 1;
	mode->active = true;
	mode->burned_size = (mode->max_player_table * mode->max_card_player) + mode->flop_size;
	g_game[COURCHEVEL] = create_game(COURCHEVEL);
}

void			init_modes(void)
{
	g_randfd = open("/dev/urandom", O_RDONLY);
	init_holdem();
	init_double_holdem();
	init_omaha();
	init_pineapple();
	init_courchevel();
}

void	end_mode(void)
{
	int		i;

	i = 0;
	while (i < NB_MODE)
	{
		if (g_mode[i].name)
			free(g_mode[i].name);
		++i;
	}
}
