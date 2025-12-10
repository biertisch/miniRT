/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/04/16 14:39:49 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
* @brief ft_calloc() allocates memory for an array of nmemb elements of size
*        bytes each and initializes all bytes in the allocated memory to zero.
*        The function returns a pointer to the allocated memory.
*
* @param nmemb The number of elements to allocate.
* @param size The size of each element in bytes.
* @return A pointer to the allocated memory, or NULL if the allocation fails.
*/
void	*ft_calloc(size_t nmemb, size_t size)
{
	void			*ptr;
	size_t			bytes_len;
	unsigned char	*uc_ptr;

	if (size && nmemb > (SIZE_MAX / size))
		return (NULL);
	bytes_len = nmemb * size;
	ptr = malloc(bytes_len);
	if (!ptr)
		return (NULL);
	uc_ptr = (unsigned char *)ptr;
	while (bytes_len--)
	{
		*uc_ptr = '\0';
		uc_ptr++;
	}
	return (ptr);
}

// int	main(void)
// {
// 	//alwaysprint: 1 to always print the result, 0 to only print on error
// 	int		alwaysprint = 1;
// 	// Test ft_calloc
// 	char	*str,*org_str;
// 	int		*arr,*org_arr;
// 	size_t	i;
// 	int		count = 5;
// 	int		size = 4;

// 	str = ft_calloc(count, size);
// 	if (str == NULL)
// 	{
// 		printf("Error: str Memory allocation failed\n");
// 		return (1);
// 	}
// 	org_str = calloc(count, size);
// 	if (org_str == NULL)
// 	{
// 		printf("Error: str Memory allocation failed\n");
// 		free(str);
// 		return (1);
// 	}

// 	if (ft_memcmp(str, org_str, count * size) != 0 || alwaysprint){
// 		for (i = 0; i < (count*size)/sizeof(char); i++)
// 		{
// 			printf("org_str:'%c';str:'%c' ", org_str[i],str[i]);
// 		}
// 		printf("\n");
// 	}
// 	arr = ft_calloc(count, size);
// 	if (arr == NULL)
// 	{
// 		printf("Error: arr Memory allocation failed\n");
// 		free(str);
// 		free(org_str);
// 		return (1);
// 	}
// 	org_arr = calloc(count, size);
// 	if (org_arr == NULL)
// 	{
// 		printf("Error: arr Memory allocation failed\n");
// 		free(str);
// 		free(org_str);
// 		free(arr);
// 		return (1);
// 	}
// 	if (ft_memcmp(arr, org_arr, count * size) != 0 || alwaysprint){
// 		for (i = 0; i < (count*size)/sizeof(int); i++)
// 		{
// 			printf("org_arr:'%d';arr:'%d' ", org_arr[i],arr[i]);
// 		}
// 	}
// 	free(str);
// 	free(org_str);
// 	free(arr);
// 	free(org_arr);
// 	return (0);
// }