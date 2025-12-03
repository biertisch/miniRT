/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/04/16 14:36:32 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief memmove() copies n bytes from memory area src to memory
 *        area dest. The memory areas may overlap: copying takes place as though
 *        the bytes in src are first copied into a temporary array that does not
 *        overlap src or dest, and the bytes are then copied from the temporary
 *        array to dest.
 * 
 * @param dest Pointer to the destination memory area.
 * @param src Pointer to the source memory area.
 * @param n The number of bytes to copy.
 * @return A pointer to dest.
 */
void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*ptrdest;
	unsigned char	*ptrsrc;

	ptrdest = (unsigned char *)dest;
	ptrsrc = (unsigned char *)src;
	if (ptrdest == ptrsrc || n == 0)
		return (dest);
	if (ptrdest < ptrsrc)
	{
		while (n--)
			*ptrdest++ = *ptrsrc++;
	}
	else
	{
		ptrdest += n;
		ptrsrc += n;
		while (n--)
			*(--ptrdest) = *(--ptrsrc);
	}
	return (dest);
}
