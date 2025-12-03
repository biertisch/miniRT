/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:46:16 by bliu              #+#    #+#             */
/*   Updated: 2025/06/16 20:37:58 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lnbr_b_fd(long nbr, const char *base, int fd)
{
	long	base_len;
	long	unbr;
	int		count;

	base_len = 0;
	count = 0;
	while (base[base_len] != '\0')
		base_len++;
	if (base_len < 2)
		return (0);
	if (nbr < 0)
	{
		write(fd, "-", 1);
		count++;
		unbr = (unsigned long)(-nbr);
	}
	else
		unbr = (unsigned long)nbr;
	if (unbr >= base_len)
		count += ft_lnbr_b_fd(unbr / base_len, base, fd);
	write(fd, &base[unbr % base_len], 1);
	return (count + 1);
}

int	ft_ulnbr_b_fd(unsigned long ulnbr, const char *base, int fd)
{
	unsigned long	base_len;
	int				count;

	base_len = 0;
	count = 0;
	while (base[base_len] != '\0')
		base_len++;
	if (base_len < 2)
		return (0);
	if (ulnbr >= base_len)
		count += ft_ulnbr_b_fd(ulnbr / base_len, base, fd);
	write(fd, &base[ulnbr % base_len], 1);
	return (count + 1);
}
