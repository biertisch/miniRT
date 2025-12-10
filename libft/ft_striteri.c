/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/04/23 14:28:45 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Applies the function ’f’ to each character of the
 *        string passed as argument, passing its index as
 *        the first argument. Each character is passed by
 *        address to ’f’ so it can be modified if necessary.
 *
 * @param s The string to iterate over.
 * @param f The function to apply to each character.
 */
void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	char	*str;

	if (s == NULL || f == NULL)
		return ;
	str = s;
	while (*str)
	{
		f(str - s, str);
		str++;
	}
}

// void	ft_upper(unsigned int i, char *c)
// {
// 	(void)i;
// 	if (*c >= 'a' && *c <= 'z')
// 		*c = *c - 32;
// }

// int	main(void)
// {
// 	char str[2];// = "womeworldasldkfjioewjrkdasf lkjasdlfjasd";
// 	ft_striteri(str, ft_upper);
// 	printf("[%s]\n", str); // Output: "HELLO, WORLD!"
// 	return (0);
// }