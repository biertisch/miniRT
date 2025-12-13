/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 15:16:49 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/13 13:03:26 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	skip_spaces(const char **s)
{
	while (**s && ft_isspace((unsigned char)**s))
		(*s)++;
}

int	check_trailing(const char *s)
{
	skip_spaces(&s);
	return (*s == '\0' || *s == '\n' || *s == '#');
}

static double	parse_fraction(const char *str, int *i, int *has_digits)
{
	double	fraction;
	double	result;

	fraction = 0.1;
	result = 0;
	while (ft_isdigit((unsigned char)str[*i]))
	{
		result += (str[*i] - '0') * fraction;
		fraction *= 0.1;
		*has_digits = 1;
		(*i)++;
	}
	return (result);
}

static double	parse_integer(const char *str, int *i, int *has_digits)
{
	double	result;

	result = 0;
	while (ft_isdigit((unsigned char)str[*i]))
	{
		result = result * 10 + (str[*i] - '0');
		*has_digits = 1;
		(*i)++;
	}
	return (result);
}

static int	parse_sign(const char *str, int *i)
{
	int	sign;

	sign = 1;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

double	ft_atof_count(const char *str, int *read)
{
	double	result;
	int		sign;
	int		has_digits;
	int		i;

	i = 0;
	sign = parse_sign(str, &i);
	has_digits = 0;
	result = parse_integer(str, &i, &has_digits);
	if (str[i] == '.')
	{
		i++;
		result += parse_fraction(str, &i, &has_digits);
	}
	if (!has_digits)
	{
		*read = 0;
		return (0);
	}
	*read = i;
	return (result * sign);
}

int	ft_atoi_count(const char *str, int *read)
{
	long	result;
	int		sign;
	int		has_digits;
	int		i;

	i = 0;
	sign = parse_sign(str, &i);
	result = 0;
	has_digits = 0;
	while (ft_isdigit((unsigned char)str[i]))
	{
		result = result * 10 + (str[i++] - '0');
		has_digits = 1;
	}
	if (!has_digits)
	{
		*read = 0;
		return (0);
	}
	*read = i;
	return ((int)(result * sign));
}

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
	int		sign;
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
