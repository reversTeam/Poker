CC = gcc

NAME = lib/libft.a

DEBUG = true

INCLUDES = -I./include

CFLAGS = -Wall -Wextra $(INCLUDES)

ifeq ($(DEBUG),true)
	CFLAGS += -g3
else
	CFLAGS += -O3
endif

LDFLAGS = -lft -Llib $(INCLUDES)

SRCS =	src/ft_atoi.c														\
		src/ft_bzero.c														\
		src/ft_findinarray.c												\
		src/ft_flagsplit.c													\
		src/ft_getnextline.c												\
		src/ft_isdigit.c													\
		src/ft_isnum.c														\
		src/ft_itoa.c														\
		src/ft_memalloc.c													\
		src/ft_memcpy.c														\
		src/ft_memdup.c														\
		src/ft_memset.c														\
		src/ft_putchar.c													\
		src/ft_putchar_fd.c													\
		src/ft_putendl.c													\
		src/ft_putendl_fd.c													\
		src/ft_putnbr.c														\
		src/ft_putnbr_fd.c													\
		src/ft_putnbr_fd.c													\
		src/ft_putnendl.c													\
		src/ft_putnendl_fd.c												\
		src/ft_putnstr.c													\
		src/ft_putnstr_fd.c													\
		src/ft_putstr.c														\
		src/ft_putstr_fd.c													\
		src/ft_sort.c														\
		src/ft_strchr.c														\
		src/ft_strcmp.c														\
		src/ft_strcpy.c														\
		src/ft_strdup.c														\
		src/ft_strlen.c														\
		src/ft_strjoin.c													\
		src/ft_strsplit.c													\
		src/ft_strstr.c														\
		src/ft_strsub.c														\
		src/ft_strtolower.c													\
		src/ft_tab_free.c													\
		src/ft_tablen.c														\
		src/ft_trim.c														\
		src/ft_strndup.c													\
		src/ft_strnjoin.c													\
		src/ft_tabadd.c														\
		src/ft_tabpop.c														\
		src/ft_isalpha.c													\
		src/ft_concatn.c

HEADERS = include/ft_common.h												\

OBJS = $(patsubst src/%,obj/%,$(SRCS:.c=.o))

OBJS_DIR = $(addsuffix .dir,$(OBJS))

SRCS_TEST = tests/main.c													\

OS = $(shell uname -s)

all: Makefile.deps $(OBJS_DIR) $(NAME)

test: tests/test.bin

run: test
	./tests/test.bin

tests/test.bin:
	gcc -o tests/test.bin $(SRCS_TEST) $(CFLAGS) $(LDFLAGS)

$(NAME): $(OBJS)
	mkdir -p $(dir $@)
	ar rc $@ $(OBJS)
	ranlib $@

-include Makefile.deps

obj/%.dir:
	@mkdir -p $(dir $@)

obj/%.o: src/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
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

.PHONY: clean fclean Makefile.deps  all re test run tests/test.bin
