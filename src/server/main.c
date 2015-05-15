#include "ft_server.h"
#include <sys/ioctl.h>
#include "ft_libsock.h"
#include <signal.h>
#include <locale.h>

static void		end_pgm(void)
{
	ft_closeserver(g_server);
	endwin();
	mysql_close(g_mysql);
	close(g_randfd);
	end_game();
	end_mode();
}

void	handler(int c)
{
	(void)c;
	end_pgm();
	exit(1);
}

static void		start_pgm(void)
{
	setlocale(LC_CTYPE, "");
	signal(SIGQUIT, handler);
	signal(SIGINT, handler);
	signal(SIGSEGV, handler);

	g_mysql = mysql_init(NULL);
	if (!g_mysql)
	{
		dprintf(2, "error : mysql_init()\n");
		exit(2);
	}

	if (!mysql_real_connect(g_mysql, BDD_HOST, BDD_USER, BDD_PASS, BDD_TABLE, 0, NULL, 0))
	{
		dprintf(2, "error : mysql_real_connect() : %s\n", mysql_error(g_mysql));
		exit(2);
	}
}

int		main(int ac, char **av)
{
	start_pgm();
	init_modes();
	add_table_in_game(g_game[HOLDEM], 100, 200);
	display_table(g_game[HOLDEM]->table_start);


	init_server(av[1]);
	end_pgm();
	(void)ac;
	return (0);
}
