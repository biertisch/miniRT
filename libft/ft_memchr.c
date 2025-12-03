/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/04/16 14:38:35 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
* @brief ft_memchr() locates the first occurrence of c (converted to a char)
*        in n bytes of the string pointed to by s. The terminating null
*        character is considered to be part of the string; therefore, if c is
*        `\0`, the function locates the terminating `\0`.
*
* @param s Pointer to the string to be searched.
* @param c The character to be located.
* @param n The number of bytes to search.
* @return A pointer to the first occurrence of c in s, or NULL if c is not
*         found in s.
*/
void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*ptr;
	size_t				i;

	ptr = (const unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (ptr[i] == (unsigned char)c)
			return ((void *)(ptr + i));
		i++;
	}
	return (NULL);
}
