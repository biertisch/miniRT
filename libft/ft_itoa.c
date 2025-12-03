/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/06/25 14:12:33 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Allocates memory (using malloc(3)) and returns 
 *        a string representing the integer received as an
 *        argument. Negative numbers must be handled.
 *
 * @param n The integer to be converted.
 * @return The string representing the integer.
 *         NULL if the allocation fails.
 */
static int	ft_numlen(int n)
{
	int		len;
	long	num;

	len = 0;
	num = n;
	if (n > 0)
		num = -num;
	else
		len++;
	while (num < 0)
	{
		num /= 10;
		len++;
	}
	return (len);
}

static void	assign_str(char *str, int num, int num_len)
{
	int		i;

	str[num_len] = '\0';
	if (num >= 0)
		num = -num;
	else
		str[0] = '-';
	if (num == 0)
		str[0] = '0';
	i = num_len - 1;
	while (num < 0)
	{
		str[i] = '0' - (num % 10);
		num /= 10;
		i--;
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	int		num_len;

	num_len = ft_numlen(n);
	str = malloc((num_len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	assign_str(str, n, num_len);
	return (str);
}

// int	main()
// {
// 	printf("%s\n", ft_itoa(0));
// 	printf("%s\n", ft_itoa(-1));
// 	printf("%s\n", ft_itoa(1));
// 	printf("%s\n", ft_itoa(-10));
// 	printf("%s\n", ft_itoa(10));
// 	printf("%s\n", ft_itoa(-42));
// 	printf("%s\n", ft_itoa(42));
// 	printf("%s\n", ft_itoa(-2147483648));
// 	printf("%s\n", ft_itoa(2147483647));
// }