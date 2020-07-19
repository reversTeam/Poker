#ifndef FT_COMMON_H
# define FT_COMMON_H

# include <unistd.h>
# include <inttypes.h>

# define GNL_BUFSIZE 1024

# define t_bool char
# define true 1
# define false 0

char		*ft_itoa(int64_t nbr);
char		*ft_strdup(const char *s);
char		*ft_strndup(const char *s, size_t n);
char		*ft_strchr(const char *s, int c);
char		*ft_strcpy(char *dest, const char *src);
char		*ft_strncpy(char *dest, char *src, size_t n);
char		*ft_strstr(char *str, const char *find);
char		*ft_strjoin(const char *s1, const char *s2);
char		*ft_strnjoin(const char *s1, size_t len1, const char *s2,
				size_t len2);
char		*ft_trim(char *s);
char		*ft_strtolower(char *s);
char		**ft_strsplit(char *str, char c);
char		**ft_flagsplit(char *str);
char		**ft_sort(char **tab, int (*cmp)());
char		*ft_strsub(char *str, size_t start, size_t len);

void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memalloc(size_t n);
void		*ft_memset(void *ptr, int c, size_t n);
void		*ft_memdup(const void *src, size_t n);
void		ft_bzero(void *ptr, size_t n);
void		ft_putchar(char c);
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *str, int fd);
void		ft_putnstr_fd(char *str, int fd, size_t n);
void		ft_putstr(char *str);
void		ft_putnstr(char *str, size_t n);
void		ft_putendl(char *str);
void		ft_putnendl(char *str, size_t n);
void		ft_putendl_fd(char *str, int fd);
void		ft_putnendl_fd(char *str, int fd, size_t n);
void		ft_putnbr(long int number);
void		ft_putnbr_fd(long int number, int fd);
void		ft_tab_free(char **tab);

size_t		ft_strlen(const char *s);

int			ft_findinarray(char *array, char c);
int			ft_getnextline(int fd, char **line);
int			ft_isnum(int c);
int			ft_tablen(char **tab);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t len);

int64_t	ft_atoi(char *number);

t_bool		ft_isdigit(char *str);
int			ft_isalpha(int c);
t_bool		ft_tabadd(char ***tab, char *str);
t_bool		ft_tabpop(char ***tab);
char		*ft_concatn(t_bool freefirst, size_t n, ...);

#endif
