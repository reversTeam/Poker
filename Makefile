CC=						gcc

NAME=					client
NAME_SERVER=			server

DEBUG=					true

INCLUDES=				-Iinclude											\
						-I/usr/local/include								\
						-Isrc/lib/include									\
						-Isrc/libsock/include								\

CFLAGS=					-Wall -Wextra $(INCLUDES)

ifeq ($(DEBUG),true)
	CFLAGS += -g3
else
	CFLAGS += -O3
endif

LDFLAGS=				$(INCLUDES) -Lsrc/lib/lib -Lsrc/libsock/lib -lft -lftsock -lncursesw -lmysqlclient

SRCS=					src/common/card_name.c								\

SRCS_SERVER=			$(SRCS)												\
						src/server/cards.c									\
						src/server/deck.c									\
						src/server/print_debug.c							\
						src/server/table.c									\
						src/server/game.c									\
						src/server/player.c									\
						src/server/mode.c									\
						src/server/round.c									\
						src/server/junk_hand.c								\
						src/server/display_table.c							\
						src/server/display_init.c							\
						src/server/cmd.c									\
						src/server/manager.c								\
						src/server/selector.c								\
						src/server/sock.c									\
						src/server/action.c									\

SRCS_CLIENT=			$(SRCS)												\

SRCS_SERVER_TEST=		$(SRCS_SERVER)
SRCS_CLIENT_TEST=		$(SRCS_CLIENT)

SRCS_SERVER_TEST+=		$(shell find tests/$(USER)/server -name "*.c")
SRCS_CLIENT_TEST+=		$(shell find tests/$(USER)/client -name "*.c")

HEADERS=				$(shell find include -name "*.h")

OBJS=					$(patsubst src/%,obj/%,$(SRCS:.c=.o))
OBJS_CLIENT=			$(patsubst src/%,obj/%,$(SRCS_CLIENT:.c=.o))
OBJS_SERVER=			$(patsubst src/%,obj/%,$(SRCS_SERVER:.c=.o))

OS=						$(shell uname -s)



all : lib Makefile.deps $(NAME) $(NAME_SERVER)

re: fclean all

lib:
	make -C src/libsock

lib_clean:
	make clean -C src/libsock

clean: lib_clean
	@rm -rf obj

fclean: clean
	rm -rf $(NAME) $(NAME_SERVER) _server _client

test: $(USER).test

run: lib Makefile.deps $(USER).test
	@rm -rf _client.dSYM _server.dSYM

$(USER).test: lib client_test server_test

server_test: $(SRCS_SERVER_TEST)
	gcc -o _server $(SRCS_SERVER_TEST) $(CFLAGS) $(LDFLAGS)

client_test: $(SRCS_CLIENT_TEST)
	gcc -o _client $(SRCS_CLIENT_TEST) $(CFLAGS) $(LDFLAGS)

$(NAME): $(OBJS_CLIENT) obj/client/main.o
	gcc -o $@ $^ $(LDFLAGS)

$(NAME_SERVER): $(OBJS_SERVER) obj/server/main.o
	gcc -o $@ $^ $(LDFLAGS)

-include Makefile.deps

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

ifeq ($(OS),Darwin)
Makefile.deps: $(SRCS_SERVER) $(HEADERS)
	makedepend -- $(INCLUDES) -- $(SRCS_SERVER) -f- > Makefile.deps
	@\vim Makefile.deps -c '%s/^src\//obj\//g' -c wq
else
Makefile.deps: $(SRCS_SERVER) $(SRCS_CLIENT) $(HEADERS)
	@makedepend -- $(INCLUDES) -- $(SRCS_SERVER) $(SRCS_CLIENT) -f- > Makefile.deps 2> /dev/null
	@\vim Makefile.deps -c '%s/^src\//obj\//g' -c wq
endif

.PHONY: clean fclean re all _server _client $(USER).test lib lib_clean
