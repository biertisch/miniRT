/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:41:19 by bliu              #+#    #+#             */
/*   Updated: 2025/10/03 21:38:25 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>
# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include <errno.h>
# include <stdio.h>
# include <stdarg.h>
# include <limits.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

# ifndef DEBUG
#  define DEBUG 1
# endif

void		debug_p_arr(char **arr);
void		debug_nstr_dprint(const int fd, ...);
int			ft_count_words(char const *s);
void		ft_exit(int exit_code, char *msg);
void		ft_free_array(char **argv);
char		*get_next_line(int fd);
int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isspace(int c);
int			ft_isalnum(int c);
int			ft_isascii(int c);
int			ft_isprint(int c);
size_t		ft_strlen(const char *str);
int			ft_arraylen(char **array);
void		*ft_memset(void *s, int c, size_t n);
void		ft_bzero(void *s, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t size);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
size_t		ft_strlcat(char *dst, const char *src, size_t size);
int			ft_toupper(int c);
int			ft_tolower(int c);
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
char		*ft_strnstr(const char *big, const char *little, size_t len);
int			ft_atoi(const char *nptr);
int			ft_str_is_int(const char *str);
long long	ft_strtoll(const char *str, int *is_valid);

void		*ft_calloc(size_t nmemb, size_t size);
char		*ft_strdup(const char *s);

char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);
char		**ft_split(char const *s, char c);
char		**ft_split_cmd(const char *cmdline);
char		**ft_split_cmd_kq(const char *cmdstr);
char		*ft_itoa(int n);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void		ft_striteri(char *s, void (*f)(unsigned int, char*));

void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
//BONUS	
t_list		*ft_lstnew(void *content);
void		ft_lstadd_front(t_list **lst, t_list *new);
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

# define BASE_10	"0123456789"
# define BASE_16	"0123456789abcdef"
# define BASE_16U	"0123456789ABCDEF"

int			ft_dprintf(const int fd, const char *format, ...);
int			debug_dprintf(const int fd, const char *format, ...);
void		ft_nstr_dprint(const int fd, ...);

size_t		ft_strlen(const char *str);
int			ft_lnbr_b_fd(long nbr, const char *base, int fd);
int			ft_ulnbr_b_fd(unsigned long ulnbr, const char *base, int fd);
int			ft_putchar_fd(char c, int fd);
int			ft_putstr_fd(char *s, int fd);

#endif
