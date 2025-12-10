/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/04/16 14:36:19 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief memcpy()  function  copies  n bytes from memory area src to memory
          area dest.  The memory areas must not overlap.  Use memmove(3)  if  the
          memory areas do overlap.
 * 
 * @param dest Pointer to the destination memory area.
 * @param src Pointer to the source memory area.
 * @param n The number of bytes to copy.
 * @return A pointer to dest.
 */
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*ptrdest;
	unsigned char	*ptrsrc;

	if (dest == src || n == 0)
		return (dest);
	ptrdest = (unsigned char *)dest;
	ptrsrc = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		ptrdest[i] = ptrsrc[i];
		i++;
	}
	return (dest);
}

// void	main()
// {
// 	char *ptr1;
// 	char *ptr2;
// 	char str1[10]	= "12345";
// 	char str2[10]	= "67890";
// 	printf("%p:%p:%p\n",ptr1,ptr2,ft_memcpy(NULL,NULL,8));
// 	printf("%p:%p:%p\n",ptr1,ptr2,memcpy(NULL,NULL,8));
// 	printf("%p:%p:%p\n",ptr1,ptr2,memcpy(ptr1,ptr2,1));

// 	ft_memcpy(str1, str2, 5);
// 	for (int i = 0; i < sizeof(str1) / sizeof(str1[0]); i++)
// 	{
// 		printf("[%d]:%c ", i, str1[i]);
// 	}
// 	printf("\n");
// }