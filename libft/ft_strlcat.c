/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/04/23 14:29:06 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief ft_strlcat() appends the string pointed to by src to the end of the
 *		  string pointed to by dst. It will append at most size - strlen(dst) - 1
 *  	  characters, NUL-terminating the result.
 * 
 * @param dst Pointer to the destination buffer.
 * @param src Pointer to the source string.
 * @param size The size of the destination buffer.
 * @return The total length of the string that was attempted to be created.
*/
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	if (size == 0)
		return (ft_strlen(src));
	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (size <= dst_len)
		return (size + src_len);
	i = 0;
	while (src[i] && (dst_len + i) < (size - 1))
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}

// #include <stdio.h>
// #include <bsd/string.h>
// int	main()
// {
// 	const char src[] = "lorem ipsum dolor sit amet\0";
// 	// size_t org_ret=strlcat(NULL, src, 1);
// 	// printf("%ld,[%s]\n",org_ret,src);

// 	size_t ft_ret=ft_strlcat(NULL, src, 1);
// 	printf("%ld,[%s]\n",ft_ret,src);
// 	// char org_dest[15] = "rrrrrrrrrrrrrr\0";
// 	// int j=16;
// 	// size_t org_ret=strlcat(org_dest, src, j);
// 	// printf("%ld,[%s],[%s]",org_ret,org_dest,src);
// // 	int errs = 0;
// // 	for(size_t j=0;j<20;j++){
// // 		char src[] = "lorem ipsum dolor sit amet\0";	
// // 		char ft_dest[15] = "rrrrrrrrrrrrrr\0";
// // 		char org_dest[15] = "rrrrrrrrrrrrrr\0";
// // 		size_t ft_ret=ft_strlcat(ft_dest, src, j);
// // printf("%ld->ft_strlcat:src length is :%ld\n",j,strlen(src));
// // 		size_t org_ret=strlcat(org_dest, src, j);
// // printf("%ld->strlcat:src length is :%ld\n",j,strlen(src));
// // 		if(org_ret!=ft_ret || strcmp(org_dest, ft_dest) != 0){
// // 			printf("size:%zu;size+len(src):%zu;exp:%zu,got:%zu;[%s]:[%s]\n"
// // 					,j,j+strlen(src),org_ret, ft_ret,org_dest,ft_dest);
// // 			errs++;
// // 		}
// // 	}
// // 	if(errs==0)
// // 		printf("All tests passed\n");
// // 	else
// // 		printf("%d tests failed\n",errs);
// }