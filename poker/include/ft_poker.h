#ifndef FT_POKER_H
# define FT_POKER_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <inttypes.h>
# include <string.h>
# include <curses.h>
# include "ft_common.h"

# define t_card				uint8_t

# define SPADE				1 // Pique
# define HEART				2 // Coeur
# define DIAMOND			4 // Carreau
# define CLUB				8 // Trefle

/*
** Card structure
** 2 last bits:  color
** 4 first bits: card value
*/

/** FOR THE CARD **/
# define NO_CARD			0
# define TWO				2
# define THREE				3
# define FOUR				4
# define FIVE				5
# define SIX				6
# define SEVEN				7
# define EIGHT				8
# define NINE				9
# define TEN				10
# define JACK				11
# define QUEEN				12
# define KING				13
# define AS					14

/** FOR THE WIN **/
# define HIGHT_CARD			0 // Carte Simple
# define ONE_PAIR			1 // Une paire
# define TWO_PAIR			2 // Deux paire
# define BRELAN				3 // Brelan
# define QUINTE				4 // Quinte
# define COLOR				5 // Couleur
# define FULL				6 // Full
# define QUADS				7 // Carrée
# define QUINTE_FLUSH		8 // Quinte flush

/** FOR THE ROLE PLAYER **/
# define SPECTATOR			0 // Spectateur
# define DEALER				1 // Dealer
# define PLAYER				2 // Joueur

/** FOR THE BIND PLAYER **/
# define NO_BLIND			0 // Pas de Blinde
# define SMALL_BLIND		1 // Petite Blinde
# define BIG_BLIND			2 // Grosse Blinde

/** FOR THE STATUS PLAYER **/
# define TO_WAIT_PLAY		0 // Attends le debut de partie
# define TO_WAIT			1 // Attends sont tours
# define TO_CALL			2 // Suivre
# define TO_RAISE			3 // Relancer
# define TO_FOLD			4 // Se coucher
# define TO_CHECK			5 // Checker
# define TO_ALL_IN			6 // Tapis

/** FOR THE SELECT GAME **/
# define NO_GAME			0 // Aucun Jeux
# define GAME_POKER			1 // Jeux Poker

# define NB_MODE			5

typedef struct s_mode		t_mode;

struct s_mode
{
	char					*name;
	int						deck_size;
	int						burned_size;
	int						flop_size;
	int						flop_open;
	int						max_player_table;
	int						max_card_player;
	t_bool					active;
	t_card					card_start;
	t_card					card_end;
};

t_mode						g_mode[NB_MODE];

#endif
