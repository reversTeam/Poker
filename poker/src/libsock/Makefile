CC = gcc

NAME = lib/libftsock.a

DEBUG = true

INCLUDES = -Iinclude -I$(LIBDIR)/include

CFLAGS = -Wall -Wextra $(INCLUDES)

ifeq ($(DEBUG),true)
	CFLAGS += -g3
else
	CFLAGS += -O3
endif

LIBDIR = ../lib

LDFLAGS = -lft -L$(LIBDIR)/lib $(INCLUDES)

SRCS =	\
		src/ft_sockaccept.c \
		src/ft_sockbind.c \
		src/ft_sockclose.c \
		src/ft_sockconnect.c \
		src/ft_sockcreate.c \
		src/ft_sockdestroy.c \
		src/ft_socklisten.c \
		src/ft_socklistenserver.c \
		src/ft_sockread.c \
		src/ft_sockreadline.c \
		src/ft_sockserver.c \
		src/ft_sockwriteline.c \
		src/ft_ssockread.c \
		src/ft_ssockwrite.c \
		src/ft_closeserver.c \

HEADERS =	\
			include/ft_libsock.h

OBJS = $(patsubst src/%,obj/%,$(SRCS:.c=.o))

OBJS_DIR = $(addsuffix .dir,$(OBJS))

OS = $(shell uname -s)

all: Makefile.deps lib $(OBJS_DIR) $(NAME)

lib:
	make -C $(LIBDIR)

lib_clean:
	make -C $(LIBDIR) clean

lib_fclean:
	make -C $(LIBDIR) clean

$(NAME): $(OBJS)
	mkdir -p $(dir $@)
	ar rc $@ $(OBJS)
	ranlib $@

-include Makefile.deps

obj/%.dir:
	@mkdir -p $(dir $@)

obj/%.o: src/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean: lib_clean
	rm -rf obj

fclean: clean
	rm -rf lib

re: fclean all

ifeq ($(OS),Darwin)
Makefile.deps: $(SRCS) $(HEADERS)
	makedepend -- $(INCLUDES) -- $(SRCS) -f- > Makefile.deps
	@\vim Makefile.deps -c '%s/src\//obj\//g' -c wq
else
Makefile.deps: $(SRCS) $(HEADERS)
	@makedepend -- $(INCLUDES) -- $(SRCS) -f- > Makefile.deps 2> /dev/null
	@\vim Makefile.deps -c '%s/src\//obj\//g' -c wq
endif

.PHONY: clean fclean all re lib lib_clean lib_fclean
