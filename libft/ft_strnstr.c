/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/04/23 14:22:10 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
* @brief ft_strnstr() locates the first occurrence of the null-terminated
*        string little in the string big, where not more than len characters
*        are searched. Characters that appear after a `\0` character are not
*        searched.
*
* @param big Pointer to the string to be searched.
* @param little Pointer to the substring to be located.
* @param len The maximum number of characters to search.
* @return A pointer to the first occurrence of little in big, or NULL if little
*         is not found in big.
*/
char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (!*little || len == 0)
		return ((char *)big);
	i = 0;
	while (big[i] && i < len)
	{
		j = 0;
		while (big[i + j] == little[j] && big[i + j] && (i + j) < len)
			j++;
		if (!little[j])
			return ((char *)&big[i]);
		i++;
	}
	return (NULL);
}
// #include <stdio.h>
// #include <bsd/string.h>
// int	main(void)
// {
// 	// //test the big is null
// 	// char	*expected = strnstr(NULL, "fake", 0);
// 	// printf("Expected: %s\n", expected);

// 	// char	*result = ft_strnstr(NULL, "fake", 0);
// 	// printf("ft_strnstr result: %s\n", result);

// 	//test the little is null
// 	char	*expected = strnstr(NULL, NULL, 0);
// 	printf("Expected: %s\n", expected);

// 	char	*result = ft_strnstr(NULL, NULL, 0);
// 	printf("ft_strnstr result: %s\n", result);
// 	return (0);
// }