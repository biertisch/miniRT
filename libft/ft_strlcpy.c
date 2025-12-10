/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/04/21 18:25:10 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief ft_strlcpy() copies up to size - 1 characters from the string pointed
 *		  to by src to the buffer pointed to by dst, NUL-terminating the result
 *		  if size is not 0.
 * 
 * @param dst Pointer to the destination buffer.
 * @param src Pointer to the source string.
 * @param size The size of the destination buffer.
 * @return The total length of the string that was attempted to be created.
*/
size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	src_len;

	i = 0;
	src_len = 0;
	if (size > 0)
	{
		while (i < (size - 1) && src[i])
		{
			dst[i] = src[i];
			i++;
			src_len++;
		}
		dst[i] = '\0';
	}
	while (src[src_len])
		src_len++;
	return (src_len);
}

// int main(void)
// {
//     char src[] = "Hello,World!";
//     char dest[10]= "1";
// 	int errs=0;
// 	printf("Test ft_strlcpy:\n");
//     // Test case 1: size = 0
//     size_t result = ft_strlcpy(dest, src, 0);
// 	if(result!=12){
// 		printf("Error: expected 12, got %zu\n", result);
// 		errs++;
// 	}
// 	if(dest[0] != '1'){
// 		printf("Error: expected 1, got %c\n", dest[0]);
// 		errs++;
// 	}
//     // Test case 2: size = 1
//     result = ft_strlcpy(dest, src, 2);
//     if(result != 12){
// 		printf("Error: expected 12, got %zu\n", result);
// 		errs++;
// 	}
// 	if(dest[0] != 'H'){
// 		printf("Error: expected H, got %c\n", dest[0]);
// 		errs++;
// 	}

// 	if(errs==0){
// 		printf("All tests passed!\n");
// 	}
//     return 0;
// }