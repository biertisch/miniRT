/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/09/17 12:44:29 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
* @brief ft_atoi() converts the initial portion of the string pointed to by
*        nptr to int representation. The function discards any whitespace
*        characters until the first non-whitespace character is found. Then,
*        it takes an optional '+' or '-' sign followed by digits. The function
*        stops converting when it reaches a non-digit character or the end of
*        the string. 
* 
* @param nptr Pointer to the string to be converted.
* @return The converted integer value.
*/
int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
		result = result * 10 + (str[i++] - '0');
	return ((int)(result * sign));
}

int	ft_str_is_int(const char *str)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	if (str[i] == '\0')
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if ((sign == 1 && result > (INT_MAX - (str[i] - '0')) / 10)
			|| (sign == -1 && result > (-1l * INT_MIN - (str[i] - '0')) / 10))
			return (0);
		result = result * 10 + (str[i++] - '0');
	}
	return ((result * sign) >= INT_MIN && (result * sign) <= INT_MAX);
}
// int	main(){
// 	int alwaysprint = 0; // Set to 1 to always print the result
// 	// Test cases
// 	const char *test_cases[] = {
// 		"42",
// 		" -42",
// 		"   42",
// 		"42abc",
// 		"abc42",
// 		"2147483647",
// 		"-2147483648",
// 		"0",
// 		NULL
// 	};
// 	for (int i = 0; test_cases[i] != NULL; i++) {
// 		int result = ft_atoi(test_cases[i]);
// 		int expected = atoi(test_cases[i]);
// 		if (result != expected || alwaysprint) {
// 			printf("Test case failed: Input: \"%s\", Expected: %d, Got: %d\n"
// 					, test_cases[i], expected, result);
// 		} 
// 	}
// 	// Additional test cases
// 	const char *additional_cases[] = {
// 		"   -42abc",
// 		"   +42abc",
// 		"   -2147483649", // Out of range
// 		"   2147483648",  // Out of range
// 		"   +0",
// 		"   -0",
// 		NULL
// 	};
// 	for (int i = 0; additional_cases[i] != NULL; i++) {
// 		int result = ft_atoi(additional_cases[i]);
// 		int expected = atoi(additional_cases[i]);
// 		if (result != expected || alwaysprint) {
// 			printf("Add case failed: Input: \"%s\", Expected: %d, Got: %d\n"
// 					, additional_cases[i], expected, result);
// 		} 
// 	}
// 	return 0;
// }