#ifndef FT_SERVER_H
# define FT_SERVER_H

# include "ft_poker.h"
# include "ft_libsock.h"
# include <mysql/mysql.h>
# include <inttypes.h>

/** BDD **/
# define BDD_HOST "mysql"
# define BDD_USER "root"
# define BDD_PASS "root"
# define BDD_TABLE "poker"


# define t_card uint8_t
# define t_cash uint64_t

/** MODE DE JEU POKER **/
# define HOLDEM				0 // Texas Hold’em
# define DOUBLE_HOLDEM		1 // Double Hold'em
# define OMAHA				2 // L'Omaha
# define PINEAPPLE			3 // Le Pineapple
# define COURCHEVEL			4 // Le Courchevel

typedef struct s_deck			t_deck;
typedef struct s_game			t_game;
typedef struct s_table			t_table;
typedef struct s_player			t_player;
typedef struct s_user			t_user;
typedef struct s_cmd			t_cmd;
typedef struct s_hand_val		t_hand_val;
typedef struct s_hand_col		t_hand_col;
typedef struct s_hand			t_hand;
typedef struct s_display		t_display;
typedef struct s_win			t_win;
typedef struct s_subwin			t_subwin;
typedef struct s_table_win		t_table_win;
typedef struct s_player_win		t_player_win;
typedef struct s_deck_win		t_deck_win;

t_game			*g_game[NB_MODE];

struct s_player
{
	int						role;
	int						status;
	int						blind;
	int						hand_value;
	char					*login;
	t_cash					mise;
	t_cash					sold;
	t_cash					cash;
	t_table					*table;
	t_card					*card;
	t_card					hand[5];
	t_user					*user;
};

struct s_hand_val
{
	int			nb;
	t_card		card[4];
};

struct s_hand_col
{
	int			nb;
	t_card		card[13];
};

struct s_hand
{
	t_deck		*all;
	t_hand_col	color[4];
	t_hand_val	value[13];
};

struct s_deck
{
	unsigned int		nb_card;
	t_card				*card;
	t_table				*table;
};

struct s_game
{
	short					mode;
	t_table					*table_start;
	t_table					*table_end;
};

struct s_table
{
	int						dealer;
	int						start;
	int						active;
	int						nb_player;
	int						nb_spectator;
	int						nb_wait;
	t_bool					start_round;
	t_cash					small_blind;
	t_cash					big_blind;
	t_cash					cash;
	t_game					*game;
	t_player				**player;
	t_deck					*deck;
	t_deck					*burned;
	t_deck					*flop;
	t_table					*next;
	t_table					*prev;
	t_display				*display;
};

int					g_randfd;

void				init_modes(void);
void				info(t_table *table, char *msg);
void				print_card(t_card c);
void				print_deck(t_deck *deck);
void				print_flop(t_deck *flop);
void				print_burned(t_deck *burned);
void				print_player(t_player *player);
void				print_table(t_table *table);
void				print_game(t_game *game);
void				add_card_to_deck(t_deck *to, t_card c);
void				remove_player(t_player *player);
void				destroy_deck(t_deck *deck);
void				show_down(t_table *table);
void				recover_players_card(t_table *table);
void				recover_player_card(t_player *player);
void				recover_flop(t_table *table);
void				recover_burned(t_table *table);
void				destroy_table(t_table *table);
void				change_player_active(t_table *table, t_player *player);
void				end_mode(void);
void				end_game(void);
void				init_server(char *port);
void				end_flop(t_table *table);
void				clean_deck(t_deck *deck);

char				*get_color_name(int c);
char				*get_value_name(int c);

int					get_color(t_card c);
int					get_value(t_card c);
int					add_player_in_table(t_table *table, t_user *user);
int					remove_player_in_table(t_table *table, t_player *player);
int					add_table_in_game(t_game *game, int small_blind, int big_blind);
int					remove_table_in_game(t_table *table);
int					add_random_card_player(t_player *player, t_deck *deck);
int					add_card_player(t_player *player, t_card card);
int					distribute_card(t_table *table);
int					get_blind(t_table *table);
int					start_round(t_table *table);
int					change_dealer(t_table *table);
int					mise_for_player(t_player *player, t_cash value);
int					player_to_check(t_player *player);
int					player_to_call(t_player *player);
int					player_to_raise(t_player *player, t_cash cash);
int					player_to_all_in(t_player *player);
int					player_to_fold(t_player *player);
int					end_round(t_table *table);
int					clean_status_for_next_card(t_table *table);
int					flop_open(t_table *table);
int					flop_add_card(t_table *table);
int					get_next_player(t_table *table, int i);

t_card				create_card(t_card color, t_card value, t_deck *deck);
t_card				get_random_card(t_deck *from, t_deck *to);

t_game				*create_game(short mode);

t_table				*create_table(t_game *game, int small_blind, int big_blind);

t_deck				*create_deck(t_table *table);
t_deck				*create_void_deck(t_table *table, int size);

t_player			*create_player(t_table *table, t_user *user);

/**
 * DISPLAY
 **/
# define t_window WINDOW

# define CONSOLE_HEIGHT		5
# define INFO_WIDTH			25

# define PADDING_TABLE		3

/** FOR DISPLAY USER **/
# define NB_PLAYER_HEIGHT	3
# define BOX_PLAYER_WIDTH	20
# define BOX_PLAYER_HEIGHT	8

# define BOX_CARD_WIDTH		2
# define BOX_CARD_HEIGHT	2

# define MARGIN_DECK		3

struct s_display
{
	t_win			*console;
	t_table_win		*table;
	t_win			*info;
	int				width;
	int				height;
};

struct s_subwin
{
	t_window		*window;
	t_window		*parent;
	int				height;
	int				width;
	int				pos_x;
	int				pos_y;
};

struct s_player_win
{
	t_window		*window;
	t_window		*parent;
	int				height;
	int				width;
	int				pos_x;
	int				pos_y;
	t_subwin		**card;
	t_subwin		*name;
	t_subwin		*mise;
	t_subwin		*sold;
	t_subwin		*statut;
};

struct s_deck_win
{
	t_window		*window;
	t_window		*parent;
	int				height;
	int				width;
	int				pos_x;
	int				pos_y;
	t_subwin		**card;
};

struct s_win
{
	t_window		*window;
	int				height;
	int				width;
	int				pos_x;
	int				pos_y;
};

struct s_table_win
{
	t_window		*window;
	int				height;
	int				width;
	int				pos_x;
	int				pos_y;
	t_player_win	**player;
	t_deck_win		*deck;
	t_deck_win		*flop;
	t_deck_win		*burned;
};

	t_display	*init_display(t_game *game);

	void		init_windows(t_display *display);
	void		init_colors(void);
	void		init_colors_windows(t_display *display);
	void		display_table(t_table *table);
	void		destroy_display(t_table *table);


# define ALEX

# define NO_SELECTOR		0
# define SELECTOR_GAME		1
# define SELECTOR_MODE		2
# define SELECTOR_BLIND		3

t_socket_server		*g_server;

struct			s_user
{
	char		*login;
	char		*read_buffer;
	char		*write_buffer;
	int			selector;
	int			game;
	t_game		*mode;
	t_table		*table;
	t_socket	*sock;
	t_player	*player;
	t_bool		is_connect;
	t_bool		illegal_msg;
	t_cash		sold;
};

struct			s_cmd
{
	char		*name;
	void		(*action)(t_user *user, char **args);
};

void	broadcast_table(t_user *user, char *msg);
void	broadcast_all_table(t_table *table, char *msg);

void	new_client(t_socket *sock);
void	error_client(t_socket *sock);
void	add_data_to_write(t_user *user, char *msg);
void	write_client(t_socket *sock);
void	read_client(t_socket *sock, char *buffer, size_t len);
void	disconnect_user(void *data);

void	connect_msg(t_user *user, char **args);
void	dispatch_msg(t_user *user);
void	quit_msg(t_user *user, char **args);
void	add_user_in_table(t_user *user, char **args);
void	add_data_to_write(t_user *user, char *msg);
void	get_games(t_user *user, char **args);
void	get_modes(t_user *user, char **args);
void	get_blinds(t_user *user, char **args);
void	select_game(t_user *user, char **args);
void	select_mode(t_user *user, char **args);
void	select_blind(t_user *user, char **args);
void	create_blind(t_user *user, char **args);
void	change_role(t_user *user, char **args);
void	action_to_call(t_user *user, char **args);
void	action_to_raise(t_user *user, char **args);
void	action_to_fold(t_user *user, char **args);
void	action_to_check(t_user *user, char **args);
void	action_to_all_in(t_user *user, char **args);


void	start(t_user *user, char **args);

MYSQL	*g_mysql;

# undef ALEX

#endif
