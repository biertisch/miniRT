/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/08/05 12:50:11 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
* @brief ft_strdup() allocates sufficient memory for a copy of the string s1,
*        does the copy, and returns a pointer to it. The pointer may subsequently
*        be used as an argument to the function free(3).
*
* @param s Pointer to the string to be duplicated.
* @return A pointer to the duplicated string, or NULL if the allocation fails.
*/
char	*ft_strdup(const char *s)
{
	char	*dup_str;
	int		len;

	len = 0;
	while (s && s[len])
		len ++;
	dup_str = malloc(len + 1);
	if (dup_str)
	{
		dup_str[len] = '\0';
		while (len > 0)
		{
			len --;
			dup_str[len] = s[len];
		}
	}
	return (dup_str);
}

// typedef struct s_test_case
// {
// 	char	*input;
// 	char	*(*ft)(const char *);
// 	char	*(*f)(const char *);
// }	t_testcase;
// #include <stdio.h>
// int main(void)
// {
// 	// alwaysprint: 1 to always print the result, 0 to only print on error
// 	int alwaysprint = 0;
// 	int	errors = 0;
// 	// Test the ft_strdup function
// 	t_testcase test_cases[] = {
// 		//test case: normal string
// 		{"Hello, World!", ft_strdup, strdup},
// 		//test case: empty string
// 		{"", ft_strdup, strdup},
// 		//test case: string with special characters
// 		{"!@#$%^&*()", ft_strdup, strdup},
// 		//test case: string with only null terminator
// 		{"Hello, \0World!\0", ft_strdup, strdup}
// 	};
// 	for (int i = 0; i < (int)(sizeof(test_cases) / sizeof(t_testcase)); i++)
// 	{
// 		char *org_str = test_cases[i].f(test_cases[i].input);
// 		char *dup_str = test_cases[i].ft(test_cases[i].input);
// 		if (strcmp(dup_str, org_str) != 0 || alwaysprint)
// 		{
// 			errors++;
// 			printf("Case[%d] Exp->[%s] \nCase[%d] got->[%s]\n\n", 
// 					i, org_str, i, dup_str);
// 		}
// 		free(dup_str);
// 		free(org_str);
// 	}
// 	if (errors == 0)
// 		printf("All tests passed!\n");
// 	else
// 		printf("%d tests failed.\n", errors);
// 	return 0;
// }