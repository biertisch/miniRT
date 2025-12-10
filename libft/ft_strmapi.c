/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/04/23 14:09:23 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Applies the function f to each character of the
 *        string s, passing its index as the first argument
 *        and the character itself as the second. A new
 *        string is created (using malloc(3)) to store the
 *        results from the successive applications of f.
 *
 * @param s The string to iterate over.
 * @param f The function to apply to each character.
 * @return The string created from the successive applications
 *         of ’f’.Returns NULL if the allocation fails.
 */
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char		*str;
	size_t		i;
	size_t		len;

	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}

// typedef struct s_test_case_strmapi
// {
// 	char	*input;
// 	char	(*f)(unsigned int, char);
// }	t_testcase;

// char	next_char(unsigned int i, char c)
// {
// 	i = i;
// 	return (c+1);
// }

// char	lower_case(unsigned int i, char c)
// {
// 	if (i % 2 == 0)
// 		return (c + 32);
// 	return (c);
// }
// int	main(){
// 	// alwaysprint: 1 to always print the result, 0 to only print on error
// 	int alwaysprint = 0;
// 	// Test the ft_strmapi function
// 	t_testcase test_cases[] = {
// 		//test case: normal string
// 		{"Hello, World!", next_char},
// 		//test case: empty string
// 		{"", next_char},
// 		//test case: string with special characters
// 		{"!@#$%^&*()", next_char},
// 		//test case: string with only null terminator
// 		{"Hello, \0World!\0", next_char}
// 	};

// 	for (int i = 0; i < (int)(sizeof(test_cases) / sizeof(t_testcase)); i++)
// 	{
// 		printf("Test case %d:\n", i + 1);
// 		char *result = ft_strmapi(test_cases[i].input, test_cases[i].f);
// 		if (result)
// 		{
// 			if (alwaysprint)
// 				printf("Result: %s\n", result);
// 			else
// 				printf("Result: %s\n", result);
// 			free(result);
// 		}
// 		else
// 		{
// 			printf("Error: Memory allocation failed.\n");
// 		}
// 	}
// 	return (0);
// }