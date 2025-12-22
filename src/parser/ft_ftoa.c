/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ftoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 17:17:03 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/22 10:04:29 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	padd_fraction(long fraction, int precision, char *str, int *i)
{
	int	digits;

	digits = 0;
	if (fraction == 0)
		digits = 1;
	while (fraction > 0)
	{
		fraction /= 10;
		digits++;
	}
	while (digits < precision)
	{
		str[(*i)++] = '0';
		digits++;
	}
}

static void	split_float(double n, int *precision, long *integer, long *fraction)
{
	long	scale;
	long	rounded;
	int		i;

	if (n < 0)
		n = -n;
	scale = 1;
	i = 0;
	while (i < *precision)
	{
		scale *= 10;
		i++;
	}
	rounded = (long)(n * scale + 0.5);
	*integer = rounded / scale;
	*fraction = rounded % scale;
}

static void	integer_to_buffer(long n, char *str, int *i)
{
	char	tmp[20];
	int		len;

	if (n == 0)
	{
		str[(*i)++] = '0';
		return ;
	}
	len = 0;
	while (n > 0)
	{
		tmp[len++] = (n % 10) + '0';
		n /= 10;
	}
	while (--len >= 0)
		str[(*i)++] = tmp[len];
}

static void	sign_to_buffer(double *n, char *str, int *i)
{
	int	sign;

	sign = 0;
	if (*n < 0)
	{
		sign = 1;
		*n = -(*n);
	}
	if (sign)
		str[(*i)++] = '-';
}

char	*ft_ftoa(double n, int precision)
{
	char	*str;
	long	integer;
	long	fraction;
	int		i;

	str = ft_calloc(50, sizeof(char));
	if (!str)
		return (0);
	i = 0;
	sign_to_buffer(&n, str, &i);
	if (precision < 0)
		precision = 0;
	if (precision > 6)
		precision = 6;
	split_float(n, &precision, &integer, &fraction);
	integer_to_buffer(integer, str, &i);
	if (precision > 0)
	{
		str[i++] = '.';
		padd_fraction(fraction, precision, str, &i);
		integer_to_buffer(fraction, str, &i);
	}
	str[i] = '\0';
	return (str);
}
