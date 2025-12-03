/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/06/16 20:11:47 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Outputs the character ’c’ to the specified file descriptor.
 *
 * @param c  The character to output.
 * @param fd The file descriptor on which to write.
 */

int	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
	return (1);
}

// int	main(void)
// {
// 	int		fd;
// 	char	c;

// 	fd = 1; // Standard output
// 	c = 'A';
// 	ft_putchar_fd(c, fd);
// 	c = '\n';
// 	ft_putchar_fd(c, fd);

// 	return (0);
// }