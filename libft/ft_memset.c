/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/04/21 17:37:19 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief memset() Fills the first `n` bytes of the memory area pointed to by `s`
 *        with the constant byte `c`.
 * 
 * @param s Pointer to the memory area to be filled.
 * @param c The byte value to fill the memory with (converted to unsigned char).
 * @param n The number of bytes to fill.
 * @return A pointer to the memory area `s`.
 */
void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	i = 0;
	ptr = (unsigned char *)s;
	while (i < n)
	{
		ptr[i] = c;
		i++;
	}
	return (s);
}

// int	main()
// {

// 	int r=1,c=255;
//     // Declare a 2D array (double array)
//     int matrix[r]; 

//     // Use memset to initialize all elements to 0
//     memset(matrix, 0, 4);
// 	memset(matrix, 2, 4);

//     // Print the matrix after memset
//     printf("Matrix(%d) after memset:\n",sizeof(matrix));
//     for (int i = 0; i < r; i++)
//     {
//         printf("%d ", matrix[i]);
// 		unsigned char *ptr = (unsigned char *)&matrix[i];
// 		printf("Memory representation of int 1:\n");
// 		for (size_t i = 0; i < sizeof(int); i++)
// 		{
// 			printf("Byte %zu: 0x%02x\n", i, ptr[i]);
// 		}
//         printf("\n");
//     }

//     // Use ft_memset to initialize all elements to -1
//     ft_memset(matrix, 0, 4);
// 	ft_memset(matrix, 1, 4);

//     // Print the matrix after ft_memset
//     printf("\nMatrix(%d) after ft_memset:\n",sizeof(matrix));
//     for (int i = 0; i < r; i++)
//     {
//         printf("%d ", matrix[i]);
// 		unsigned char *ptr = (unsigned char *)&matrix[i];
// 		printf("Memory representation of int 1:\n");
// 		for (size_t i = 0; i < sizeof(int); i++)
// 		{
// 			printf("Byte %zu: 0x%02x\n", i, ptr[i]);
// 		}
//         printf("\n");
//     }

// 	// char *ptr = NULL;
//     // // Attempt to use memset with a NULL pointer
//     // ft_memset(ptr, 'A', 5);

// 	// char ft_str[10];
// 	// // ft_str[0]='A';
// 	// // ft_str[1]='B';
// 	// // ft_str[2]='C';
// 	// char orig_str[10];
// 	// orig_str[0]='F';
// 	// orig_str[1]='G';
// 	// orig_str[2]='H';

// 	// printf("ft_str:[%s]%d\n", ft_str,ft_strlen(ft_str));
// 	// printf("orig_str:[%s]%d\n", orig_str,ft_strlen(orig_str));

// 	// ft_memset(ft_str, 'A', 2);
// 	// memset(orig_str, 'A', 2);

// 	// printf("ft_str:%s\n", ft_str);
// 	// printf("orig_str:%s\n", orig_str);

// 	// if(ft_memcmp(ft_str, orig_str, 3) != 0)
// 	// {
// 	// 	printf("ft_memset: ERROR %d:%d\n",ft_memcmp(ft_str, orig_str, 3),
// memcmp(ft_str, orig_str, 3) );
// 	// }
// 	return 0;
// }