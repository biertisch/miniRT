/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 17:23:41 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/13 17:26:12 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
