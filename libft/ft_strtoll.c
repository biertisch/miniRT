/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 18:17:09 by bliu              #+#    #+#             */
/*   Updated: 2025/09/17 12:23:25 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

long long	ft_strtoll_cacul(const char *str, int sign,
	unsigned long long up_range, int *is_valid)
{
	long long	result;
	int			digit;

	digit = 0;
	result = 0;
	while (*str && *is_valid)
	{
		if (*str >= '0' && *str <= '9')
		{
			digit = *str - '0';
			if ((unsigned long long)result > (up_range - digit) / 10)
			{
				*is_valid = 0;
				return (sign * up_range);
			}
			result = result * 10 + digit;
		}
		else
			*is_valid = 0;
		str++;
	}
	return (result * sign);
}

long long	ft_strtoll(const char *str, int *is_valid)
{
	int					sign;
	unsigned long long	up_range;

	sign = 1;
	*is_valid = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (sign == 1)
		up_range = LLONG_MAX;
	else
		up_range = sign * LLONG_MIN;
	if (ft_strlen(str) == 0)
	{
		*is_valid = 0;
		return (0);
	}
	return (ft_strtoll_cacul(str, sign, up_range, is_valid));
}
