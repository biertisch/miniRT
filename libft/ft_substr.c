/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/04/21 19:18:59 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
* @brief ft_substr() allocates (with malloc(3)) and returns a substring from the
*        string s.The substring begins at index start and is of maximum size len
*        If start and len are out of range, the function returns an empty string.
*        If the allocation fails, the function returns NULL.
*
* @param s The original string from which to create the substring.
* @param start The starting index of the substring within 's'.
* @param len The maximum length of the substring.
* @return A pointer to the allocated substring, or NULL if the allocation fails.
*/
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	substr = malloc(sizeof(char) * (len + 1));
	if (!substr)
		return (NULL);
	ft_strlcpy(substr, s + start, len + 1);
	return (substr);
}

// typedef struct s_test_case_substr
// {
// 	char	*input;
// 	unsigned int	start;
// 	size_t	len;
// 	char	*(*ft)(char const *, unsigned int, size_t);
// 	char	*expected;
// }	t_testcase;

// int	main(void)
// {
// 	//alwaysprint: 1 to always print the result, 0 to only print on error
// 	int		alwaysprint = 0;
// 	// Test the ft_substr function
// 	t_testcase test_cases[] = {
// 		// Test case: normal string
// 		{"Hello, World!", 7, 5, ft_substr, "World"},
// 		// Test case: empty string
// 		{"", 0, 0, ft_substr, ""},
// 		// Test case: string with special characters
// 		{"!@#$%^&*()", 2, 5, ft_substr, "#$%^&"},
// 		// Test case: string with only null terminator
// 		{"Hello, \0World!\0", 7, 5, ft_substr, ""}
// 	};

// 	for (size_t i = 0; i < (int)(sizeof(test_cases) / sizeof(t_testcase)); i++)
// 	{
// 		char *dup_str = test_cases[i].ft(test_cases[i].input,
// 						 test_cases[i].start, test_cases[i].len);
// 		if (strcmp(dup_str, test_cases[i].expected) != 0 ||alwaysprint)
// 		{
// 			printf("Error:expected->%s but got->%s\n", test_cases[i].expected
// 					, dup_str);
// 		}
// 		free(dup_str);
// 	}
// 	return (0);
// }
