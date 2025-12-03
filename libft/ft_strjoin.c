/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/04/16 14:40:41 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
* @brief ft_strjoin() allocates (with malloc(3)) and returns a new string,
*        which is the result of the concatenation of s1 and s2.
*
* @param s1 The prefix string.
* @param s2 The suffix string.
* @return A pointer to the newly allocated string,
*         or NULL if the allocation fails.
*/
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	len1;
	size_t	len2;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, len1 + 1);
	ft_strlcat(str, s2, len1 + len2 + 1);
	return (str);
}

// typedef struct s_test_case_strjoin
// {
// 	char	*str1;
// 	char	*str2;
// 	char	*expected;
// }	t_test_case_strjoin;

// int	main(){
// 	//int alwaysprintf = 1;

// 	t_test_case_strjoin test_cases[] = {
// 		{"Hello, ", "world!", "Hello, world!"},
// 		{"", "world!", "world!"},
// 		{"Hello, ", "", "Hello, "},
// 		{"", "", ""},
// 		{"Hello, ", NULL, "Hello, "},
// 		{NULL, "world!", "world!"},
// 		{NULL, NULL, NULL},
// 	};
// 	for (size_t i = 0; i < sizeof(test_cases) / sizeof(t_test_case_strjoin);
// 		 i++)
// 	{
// 		char *result = ft_strjoin(test_cases[i].str1, test_cases[i].str2);
// 		if(result == NULL)
// 		{
// 			if (test_cases[i].expected == NULL)
// 				printf("Test case %zu passed: both are NULL\n", i);
// 			else
// 				printf("Test case %zu failed: expected '%s', got NULL\n", i
// 						, test_cases[i].expected);
// 		}
// 		else if (strcmp(result, test_cases[i].expected) == 0)
// 			printf("Test case %zu passed: s1+s2:[%s]+[%s]='%s'\n",
// 				 i,test_cases[i].str1,test_cases[i].str2, result);
// 		else
// 			printf("Test case %zu failed: expected '%s', got '%s'\n", i
// 					, test_cases[i].expected, result);

// 		free(result);
// 	}
// 	return (0);
// }