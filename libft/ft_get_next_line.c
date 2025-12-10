/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 21:45:47 by bliu              #+#    #+#             */
/*   Updated: 2025/10/09 21:29:51 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_gnl_strjoin(char *s1, char c)
{
	int		len;
	int		i;
	char	*new_l;

	i = 0;
	len = ft_strlen(s1);
	new_l = malloc(len + 2);
	if (!new_l)
	{
		free(s1);
		return (NULL);
	}
	while (i < len)
	{
		new_l[i] = s1[i];
		i++;
	}
	new_l[len] = c;
	new_l[len + 1] = '\0';
	free(s1);
	return (new_l);
}

char	*get_next_line(int fd)
{
	char	buf;
	char	*line;

	line = NULL;
	buf = 0;
	while (read(fd, &buf, 1) > 0)
	{
		line = ft_gnl_strjoin(line, buf);
		if (!line)
			return (NULL);
		if (buf == '\n')
			break ;
	}
	return (line);
}
