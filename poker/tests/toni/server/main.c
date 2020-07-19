#include "ft_server.h"
#include <stdio.h>
#include <fcntl.h>


int		main(void)
{
	t_deck	*d;
	t_burned	*b;

	b = malloc(sizeof(t_burned));
	b->nb_card = 0;
	if (!(d = create_deck()))
		return (1);
	g_randfd = open("/dev/random", O_RDONLY);
	print_deck(d);
	get_random_card(d, b);
	get_random_card(d, b);
	get_random_card(d, b);
	print_deck(d);
	print_card(b->card[0]);
	print_card(b->card[1]);
	print_card(b->card[2]);
	recover_cards(d, b, NULL);
	print_deck(d);
	return (0);
}
