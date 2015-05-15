#include "ft_server.h"

int		main(void)
{
	t_game		*game;
	t_table		*table;

	init_modes();

	game = create_game(HOLDEM);
	add_table_in_game(game);

	table = game->table_start;
	add_player_in_table(table, "triviere", 666);
	table->player[0]->role = DEALER;
	add_player_in_table(table, "amerle", 555);
	table->player[1]->role = SMALL_BLIND;
	add_player_in_table(table, "thrivier", 444);
	table->player[2]->role = BIG_BLIND;
	add_player_in_table(table, "aguilbauu", 333);
	table->player[3]->role = OTHER;
	add_player_in_table(table, "ssoulier", 333);
	table->player[4]->role = SPECTATOR;

	distribute_card(table);

	print_game(game);
	remove_table_in_game(game, game->table_start);
	remove_table_in_game(game, game->table_start);
	free(game);

	return (0);
}
