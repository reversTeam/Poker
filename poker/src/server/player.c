#include "ft_server.h"

t_player		*create_player(t_table *table, t_user *user)
{
	int				i;
	t_player		*player;
	t_mode			mode;

	i = 0;
	player = (t_player*) malloc(sizeof(t_player));
	if (!player)
		return (NULL);
	bzero(player, sizeof(t_player));
	mode = g_mode[table->game->mode];
	player->login = strdup(user->login);
	player->sold = 0;
	player->table = table;
	player->mise = 0;
	player->cash = 0;
	player->user = user;
	player->role = SPECTATOR;
	player->status = TO_WAIT;
	player->blind = NO_BLIND;
	player->hand_value = HIGHT_CARD;
	player->card = (t_card*) malloc(sizeof(t_card) * mode.max_card_player);
	if (!player->card)
	{
		free(player);
		return (NULL);
	}
	while (i < mode.max_card_player)
	{
		player->card[i] = NO_CARD;
		++i;
	}
	user->player = player;
	return (player);
}

void		remove_player(t_player *player)
{
	t_table		*table;

	if (player->table)
	{
		table = player->table;
		broadcast_all_table(table, "DISCONNECT ");
		broadcast_all_table(table, player->login);
		broadcast_all_table(table, "\n");
		if (player->role != SPECTATOR)
		{
			if (player->status != TO_WAIT_PLAY)
			{
				table->nb_player--;
				recover_player_card(player);
				change_player_active(table, NULL);
			}
			else
				table->nb_wait--;
		}
		else
			table->nb_spectator--;
		display_table(table);
	}
	free(player->card);
	free(player->login);
	free(player);
}

int			add_card_player(t_player *player, t_card card)
{
	int		i;
	char	*value;

	i = 0;
	while (i < g_mode[player->table->game->mode].max_card_player)
	{
		if (!player->card[i])
		{
			value = get_value_name(get_value(card));
			player->card[i] = card;
			add_data_to_write(player->user, "YOUR_CARD ");
			add_data_to_write(player->user, value);
			add_data_to_write(player->user, " ");
			add_data_to_write(player->user, get_color_name(get_color(card)));
			add_data_to_write(player->user, "\n");
			free(value);
			return (0);
		}
		++i;
	}
	return (1);
}

int			mise_for_player(t_player *player, t_cash value)
{
	if (player)
	{
		if (player->role == SPECTATOR || player->sold <= 0)
			return (-1);
		if (player->sold > value)
		{
			player->cash += value;
			player->sold -= value;
			return (0);
		}
		else
		{
			player_to_all_in(player);
			return (1);
		}
	}
	else
		return (-1);
}

int			add_random_card_player(t_player *player, t_deck *deck)
{
	t_card		card;

	if (deck)
	{
		if (player)
		{
			if (player->role)
			{
				card = get_random_card(deck, NULL);
				if (!card)
					return (-1);
				if (add_card_player(player, card))
				{
					add_card_to_deck(deck, card);
					return (1);
				}
				return (0);
			}
			else
				return (2);
		}
		return (-2);
		}
	return (-3);
}

int			player_to_fold(t_player *player)
{
	if (player && player->table && player->role != SPECTATOR)
	{
		recover_player_card(player);
		player->status = TO_FOLD;
		add_data_to_write(player->user, "ACTION_SUCCESS FOLD\n");
		broadcast_table(player->user, "ACTION_PLAYER ");
		broadcast_table(player->user, player->login);
		broadcast_table(player->user, " TO_FOLD\n");
		change_player_active(player->table, player);
		return (0);
	}
	else
		return (-1);
}

int			player_to_call(t_player *player)
{
	t_cash		mise;
	t_table		*table;

	table = player->table;
	if (player->role == SPECTATOR || player->sold <= 0 || player->status == TO_FOLD)
		return (-1);
	mise = table->cash - player->cash;
	player->status = TO_CALL;
	
	if (mise_for_player(player, mise) == 0)
	{
		add_data_to_write(player->user, "ACTION_SUCCESS CALL\n");
		broadcast_table(player->user, "ACTION_PLAYER ");
		broadcast_table(player->user, player->login);
		broadcast_table(player->user, " TO_CALL\n");
		change_player_active(player->table, player);
	}
	return (0);
}

int			player_to_all_in(t_player *player)
{
	t_table		*table;

	table = player->table;
	if (player->role == SPECTATOR || player->sold <= 0 || player->status == TO_FOLD)
		return (-1);
	player->cash += player->sold;
	player->sold = 0;
	player->status = TO_ALL_IN;
	if (table->cash < player->cash)
		table->cash = player->cash;
	add_data_to_write(player->user, "ACTION_SUCCESS ALL_IN\n");
	broadcast_table(player->user, "ACTION_PLAYER ");
	broadcast_table(player->user, player->login);
	broadcast_table(player->user, " TO_ALL_IN\n");
	change_player_active(player->table, player);
	return (0);
}

int			player_to_raise(t_player *player, t_cash cash)
{
	t_table		*table;
	char		*tmp;

	table = player->table;
	if (player->role == SPECTATOR || player->sold <= 0 || player->status == TO_FOLD)
		return (-1);
	player->status = TO_RAISE;
	if (mise_for_player(player, cash + (table->cash - player->cash)) == 0)
	{
		table->cash += cash;
		tmp = ft_itoa(cash);
		add_data_to_write(player->user, "ACTION_SUCCESS RAISE ");
		add_data_to_write(player->user, tmp);
		add_data_to_write(player->user, "\n");
		broadcast_table(player->user, "ACTION_PLAYER ");
		broadcast_table(player->user, player->login);
		broadcast_table(player->user, " TO_RAISE ");
		broadcast_table(player->user, tmp);
		broadcast_table(player->user, "\n");
		change_player_active(player->table, player);
		free(tmp);
	}
	return (0);
}

int			player_to_check(t_player *player)
{
	t_table		*table;

	table = player->table;
	if (player->role == SPECTATOR || player->sold <= 0 || player->status == TO_FOLD)
		return (-1);
	if (player->cash == table->cash)
	{
		player->status = TO_CHECK;
		add_data_to_write(player->user, "ACTION_SUCCESS CHECK\n");
		broadcast_table(player->user, "ACTION_PLAYER ");
		broadcast_table(player->user, player->login);
		broadcast_table(player->user, " TO_CHECK\n");
		change_player_active(player->table, player);
		return (0);
	}
	return (1);
}
