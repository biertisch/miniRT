/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/04/16 14:37:17 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
*  @brief ft_toupper() converts lowercase letters to uppercase.
*		 If the character is not a lowercase letter, it is returned unchanged.
*
*  @param c The character to be converted.
*  @return The value returned is that of the converted letter, 
*			or c if the conver‐sion was not possible.
*/
int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

// int	main(){
// 	for(int j=0; j < 256; j++){
// 		if(ft_toupper(j)!=toupper(j)){
// 			printf(" ft_toupper ret:[%d]:[%d] \n",ft_toupper(j),toupper(j));
// 			if(isprint(j)){
// 				printf("ft_toupper:%d[%c] [%c]:[%c] \n"
// 						,j,j,ft_toupper(j),toupper(j));
// 			}else{
// 				printf("ft_toupper:%d [%02X]:[%02X] \n"
// 						,j,ft_toupper(j),toupper(j));
// 			}
// 		}
// 	}
// }