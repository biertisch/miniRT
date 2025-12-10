/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:52:22 by bliu              #+#    #+#             */
/*   Updated: 2025/04/16 19:14:59 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Allocates memory (using malloc(3)) and returns
 *        a new node. The ’content’ member variable is
 *        initialized with the given parameter ’content’.
 *        The variable ’next’ is initialized to NULL.
 *
 * @param content The content to store in the new node.
 * @return A pointer to the new node.
 */
t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}

// int	main(void)
// {
// 	t_list	*node;

// 	node = ft_lstnew("Hello, World!");
// 	if (node)
// 	{
// 		printf("Node content: %s\n", (char *)node->content);
// 		free(node);
// 	}
// 	return (0);
// }
