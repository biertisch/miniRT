/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/04/16 14:35:59 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief bzero()  function  erases  the  data  in the n bytes of the memory
 *		  starting at the location pointed to by s, by writing zeros (bytes  con‐
 *		  taining '\0') to that area.
 * 
 * @param s Pointer to the memory area to be filled.
 * @param n The number of bytes to fill.
 * @return none.
 */
void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	while (n--)
	{
		*ptr = '\0';
		ptr++;
	}
}

// int	main(int argc, char *argv[])
// {
// 	(void)argc;
// 	(void)argv;
// 	int ints[5];
// 	int ints2[5]= {1,2,3,4,5};

// 	bzero(ints, sizeof(ints));
// 	for (int i = 0; i < sizeof(ints) / sizeof(int); i++)
// 	{
// 		printf("ints1[%d]:%d,\n", i, ints[i]);
// 	}

// 	bzero(ints2, sizeof(ints2)-4);
// 	for (int i = 0; i < sizeof(ints2) / sizeof(int); i++)
// 	{
// 		printf("ints2[%d]:%d,\n", i, ints2[i]);
// 	}

// 	// Test case 1: Fill a string with zeros
// 	char str1[11]	= "Hello W";
// 	char str2[10]	= "Hello Y";
// 	char *ptrstr=NULL;

// 	//ft_bzero(ptrstr,5);

// 	bzero(str1, 6);
// 	ft_bzero(str2, 6);
// 	for(int j=0;j<sizeof(str1)/sizeof(str1[0]);j++)
// 	{
// 		printf("[%d]:%c,", j, str1[j]);
// 	}
// 	printf("\n");
// 	for(int j=0;j<sizeof(str2)/sizeof(str2[0]);j++)
// 	{
// 		printf("[%d]:%c,", j, str2[j]);
// 	}
// 	printf("\n");
// 	//if(ft_memcmp(str1, str2, 5)!=0)
// 		printf("str1:[%s],str2:[%s];ftmemcmp:%d,memcmp:%d\n", 
// str1, str2,ft_memcmp(str1, str2, 8),memcmp(str1, str2, 8));
// }