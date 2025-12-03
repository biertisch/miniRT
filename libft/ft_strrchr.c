/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/04/16 14:38:06 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
* @brief ft_strrchr() locates the last occurrence of c (converted to a char)
*        in the string pointed to by s. The terminating null character is
*        considered to be part of the string; therefore, if c is `\0`, the
*        function locates the terminating `\0`.
*
* @param s Pointer to the string to be searched.
* @param c The character to be located.
* @return A pointer to the last occurrence of c in s, or NULL if c is not
*         found in s.
*/
char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*last_occurrence;

	i = 0;
	last_occurrence = NULL;
	while (s[i])
	{
		if (s[i] == (char)c)
			last_occurrence = (char *)&s[i];
		i++;
	}
	if (c == '\0')
		return ((char *)&s[i]);
	return (last_occurrence);
}

// int	main()
// {
// 	char	*str = "Hello, World!";
// 	int		c = 'o';
// 	char	*ft_ret,*org_ret;
// 	// alwaysprint: 1 to always print the result, 0 to only print on error
// 	int		alwaysprint = 0;

// 	// Test with a character that exists in the string
// 	ft_ret = ft_strrchr(str, c);
// 	org_ret = strrchr(str, c);
// 	if (ft_ret!=org_ret || alwaysprint)
// 		printf("Error:expected->'%p' but got->'%p'\n", org_ret, ft_ret);

// 	// Test with a character that does not exist in the string
// 	c = 'z';
// 	ft_ret = ft_strrchr(str, c);
// 	org_ret = strrchr(str, c);
// 	if (ft_ret!=org_ret || alwaysprint)
// 		printf("Error:expected->'%p' but got->'%p'\n", org_ret, ft_ret);

// 	// Test with the null terminator
// 	c = '\0';
// 	ft_ret = ft_strrchr(str, c);
// 	org_ret = strrchr(str, c);
// 	if (ft_ret!=org_ret || alwaysprint)
// 		printf("Error:expected->'%p' but got->'%p'\n", org_ret, ft_ret);
// 	// Test with an empty string
// 	str = "";
// 	ft_ret = ft_strrchr(str, c);
// 	org_ret = strrchr(str, c);
// 	if (ft_ret!=org_ret || alwaysprint)
// 		printf("Error:expected->'%p' but got->'%p'\n", org_ret, ft_ret);
// 	// Test with a string containing only the null terminator
// 	str = "\0";
// 	ft_ret = ft_strrchr(str, c);
// 	org_ret = strrchr(str, c);
// 	if (ft_ret!=org_ret || alwaysprint)
// 		printf("Error:expected->'%p' but got->'%p'\n", org_ret, ft_ret);
// }