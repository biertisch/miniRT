/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/06/16 20:10:36 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Outputs the string ’s’ to the specified file descriptor.
 * 
 * @param s  The string to output.
 * @param fd The file descriptor on which to write.
 */

int	ft_putstr_fd(char *s, int fd)
{
	int	count;

	count = 0;
	if (s)
	{
		count = ft_strlen(s);
		write(fd, s, count);
	}
	else
	{
		count = 6;
		write(fd, "(null)", count);
	}
	return (count);
}
/**

void	ft_putstr_fd(char *s, int fd)
{
	if (s)
		write(fd, s, ft_strlen(s));
}
 */
// int	main(void)
// {
// 	char	*str;
// 	int		fd;
// 	str = "Hello\tWorld!\n";
// 	fd = 1;
// 	ft_putstr_fd(str, fd);
// 	return (0);
// }
