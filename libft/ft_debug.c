/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_debug.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 09:50:59 by bliu              #+#    #+#             */
/*   Updated: 2025/09/25 15:08:32 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	debug_p_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
	{
		debug_nstr_dprint(1, "Debug:[", arr[i], "]\n", NULL);
		i++;
	}
}

void	debug_nstr_dprint(const int fd, ...)
{
	va_list	args;
	char	*pstr;
	char	*tstr;
	char	*str;

	str = NULL;
	va_start(args, fd);
	pstr = va_arg(args, char *);
	while (pstr != NULL)
	{
		tstr = str;
		str = ft_strjoin(str, pstr);
		free(tstr);
		if (!str)
		{
			va_end(args);
			return ;
		}
		pstr = va_arg(args, char *);
	}
	va_end(args);
	if (DEBUG)
		write(fd, str, ft_strlen(str));
	free(str);
}
