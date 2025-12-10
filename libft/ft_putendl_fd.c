/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/06/17 13:32:54 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Outputs the string ’s’ to the specified file descriptor 
 *        followed by a newline.
 *
 * @param s  The string to output.
 * @param fd The file descriptor on which to write.
 */
void	ft_putendl_fd(char *s, int fd)
{
	char	*str;

	if (s == NULL)
		return ;
	str = s;
	while (*str)
	{
		write(fd, str, 1);
		str++;
	}
	write(fd, "\n", 1);
}

// int	main(void)
// {
// 	char	*str;
// 	int		fd;

// 	str = NULL;
// 	fd = 1;
// 	ft_putendl_fd(str, fd);
// 	return (0);
// }
