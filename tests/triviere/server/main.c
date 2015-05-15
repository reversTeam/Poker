#include "ft_server.h"

int		main(void)
{
	t_game		*game;
	t_table		*table;

	init_modes();

	game = create_game(HOLDEM);
	add_table_in_game(game, 100, 200);

	table = game->table_start;

	add_player_in_table(table, "triviere", 1500);
	table->player[0]->role = PLAYER;
	add_player_in_table(table, "amerle", 1500);
	table->player[1]->role = PLAYER;
	add_player_in_table(table, "thrivier", 1500);
	table->player[2]->role = PLAYER;
	add_player_in_table(table, "aguilbauu", 1500);
	table->player[3]->role = PLAYER;
	add_player_in_table(table, "ssoulier", 1500);
	table->player[4]->role = PLAYER;
	add_player_in_table(table, "Thomas", 1500);
	table->player[5]->role = PLAYER;


//	remove_player_in_table(table, table->player[1]);
//	remove_player_in_table(table, table->player[2]);
//	remove_player_in_table(table, table->player[3]);
//	remove_player_in_table(table, table->player[4]);

	start_round(table);

	player_to_call(table, table->player[3]);
	player_to_raise(table, table->player[4], 40);
	player_to_call(table, table->player[5]);
	player_to_fold(table->player[0]);
	player_to_check(table, table->player[2]);
	
	print_game(game);

	flop_open(table);
	print_game(game);

	flop_add_card(table);
	print_game(game);

	flop_add_card(table);
	print_game(game);

	end_round(table);
	print_game(game);

	start_round(table);
	print_game(game);

	remove_table_in_game(game, game->table_start);
	remove_table_in_game(game, game->table_start);
	free(game);


	return (0);
}
