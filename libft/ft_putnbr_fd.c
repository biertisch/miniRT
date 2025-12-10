/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/04/21 20:54:05 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Outputs the integer ’n’ to the specified file descriptor.
 *
 * @param n  The integer to be outputted.
 * @param fd The file descriptor on which to write.
 */

void	ft_putnbr_fd(int n, int fd)
{
	long	ln;

	ln = n;
	if (ln < 0)
	{
		ft_putchar_fd('-', fd);
		ln = -ln;
	}
	if (ln >= 10)
	{
		ft_putnbr_fd(ln / 10, fd);
		ft_putchar_fd('0' + (ln % 10), fd);
	}
	else
		ft_putchar_fd('0' + ln, fd);
}

// int	main(void)
// {
// 	int	n;
// 	int	fd;

// 	n = 42;
// 	fd = 1;
// 	ft_putnbr_fd(n, fd);
// 	return (0);
// }
