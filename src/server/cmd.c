#include "ft_server.h"
#include "ft_common.h"

t_cmd			g_cmds[] =
{
	{ "CONNECT",		connect_msg },
	{ "QUIT",			quit_msg },
	{ "GET_GAMES",		get_games },
	{ "GET_MODES",		get_modes },
	{ "GET_BLINDS",		get_blinds },
	{ "SELECT_GAME",	select_game },
	{ "SELECT_MODE",	select_mode },
	{ "SELECT_BLIND",	select_blind },
	{ "CREATE_BLIND",	create_blind },
	{ "CHANGE_ROLE",	change_role },
	{ "ADDTABLE",		add_user_in_table },
	{ "TO_CALL",		action_to_call },
	{ "TO_RAISE",		action_to_raise },
	{ "TO_CHECK",		action_to_check },
	{ "TO_FOLD",		action_to_fold },
	{ "TO_ALL_IN",		action_to_all_in },
};

t_bool	check_login(char *login)
{
	size_t	i;

	i = 0;
	while (login[i])
	{
		if (!(ft_isnum(login[i]) || ft_isalpha(login[i]) || ft_findinarray(" _.-", login[i]) != -1))
			return (false);
		++i;
	}
	return (i < 20 ? true : false);
}

void	connect_msg(t_user *user, char **args)
{
	char		*msg;
	char		*tmp;
	MYSQL_RES	*res = NULL;

	if (args[0] && args[1] && !args[2] && check_login(args[0]) && !user->is_connect)
	{
		msg = "SELECT sold FROM users WHERE username = '";
		msg = ft_strjoin(msg, args[0]);
		tmp = msg;
		msg = ft_strjoin(msg, "' AND pass = '");
		free(tmp);
		tmp = msg;
		msg = ft_strjoin(msg, args[1]);
		free(tmp);
		tmp = msg;
		msg = ft_strjoin(msg, "'");
		free(tmp);
		
		if (mysql_query(g_mysql, msg) || !(res = mysql_store_result(g_mysql)))
		{
			if (res)
				free(res);
			wprintw(user->table->display->info->window, "error mysql : %s\n", mysql_error(g_mysql));
		}
		else if (mysql_num_rows(res) == 1)
		{
			MYSQL_ROW row;
			row = mysql_fetch_row(res);
			user->sold = ft_atoi(row[0]);
			user->login = strdup(args[0]);
			user->is_connect = true;
			mysql_free_result(res);
			free(msg);
			return ;
		} else if (res)
			mysql_free_result(res);
		free(msg);
	}
	user->illegal_msg = true;
}

void	quit_msg(t_user *user, char **args)
{
	(void)args;
	user->illegal_msg = true;
}

void	dispatch_cmd(t_user *user, char **args)
{
	size_t	i;

	i = 0;
	while (i < sizeof(g_cmds) / sizeof(t_cmd))
	{
		if (!strcmp(g_cmds[i].name, args[0]))
		{
			if (strcmp("CONNECT", args[0]) && !user->is_connect)
				break ;
			g_cmds[i].action(user, args + 1);
			return ;
		}
		++i;
	}
	user->illegal_msg = true;
}

void	dispatch_msg(t_user	*user)
{
	char	*find;
	char	*msg;
	char	**args;

	while ((find = strchr(user->read_buffer, '\n')) && !user->illegal_msg)
	{
		*find = 0;
		msg = user->read_buffer;
		args = ft_flagsplit(msg);
		if (args && args[0])
		{
			dispatch_cmd(user, args);
			ft_tab_free(args);
		}
		else
			user->illegal_msg = true;
		user->read_buffer = strdup(find + 1);
		free(msg);
	}
}
