/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/04/16 14:38:51 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

/**
* @brief ft_memcmp() compares the first n bytes of the memory areas s1 and s2.
*
* @param s1 Pointer to the first memory area to be compared.
* @param s2 Pointer to the second memory area to be compared.
* @param n The number of bytes to compare.
* @return An integer less than, equal to, or greater than zero if s1 is found,
*         respectively, to be less than, to match, or be greater than s2.
*/
int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*ptrs1;
	unsigned char	*ptrs2;

	ptrs1 = (unsigned char *)s1;
	ptrs2 = (unsigned char *)s2;
	while (n--)
	{
		if (*ptrs1 != *ptrs2)
			return (*ptrs1 - *ptrs2);
		ptrs1++;
		ptrs2++;
	}
	return (0);
}

// #include <stdio.h>
// #include <string.h>

// int main(void)
// {
//     char str1[] = "Hello";
//     char str2[] = "Hello";
//     char str3[] = "Hellp";

//     // Test case 1: Identical strings
//     printf("ft_memcmp(str1, str2, 5) = %d\n", ft_memcmp(str1, str2, 5));
//     printf("memcmp(str1, str2, 5) = %d\n", memcmp(str1, str2, 5));

//     // Test case 2: Different strings
//     printf("ft_memcmp(str1, str3, 5) = %d\n", ft_memcmp(str1, str3, 5));
//     printf("memcmp(str1, str3, 5) = %d\n", memcmp(str1, str3, 5));

//     // Test case 3: Zero length
//     printf("ft_memcmp(str1, str3, 0) = %d\n", ft_memcmp(str1, str3, 0));
//     printf("memcmp(str1, str3, 0) = %d\n", memcmp(str1, str3, 0));

//     return 0;
// }