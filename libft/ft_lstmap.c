/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 19:02:56 by bliu              #+#    #+#             */
/*   Updated: 2025/04/16 20:49:30 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Iterates through the list ’lst’, applies the
 *        function ’f’ to each node’s content, and creates
 *        a new list resulting of the successive applications
 *        of the function ’f’. The ’del’ function is used to
 *        delete the content of a node if needed.
 * 
 * @param lst The address of a pointer to a node.
 * @param f   The address of the function applied to each node’s content.
 * @param del The address of the function used to delete a node's content
 *            if needed.
 * @return The new list. NULL if the allocation fails.
 */
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*current;

	if (!lst || !f)
		return (NULL);
	new_list = ft_lstnew(f(lst->content));
	if (!new_list)
		return (NULL);
	current = new_list;
	while (lst->next)
	{
		lst = lst->next;
		current->next = ft_lstnew(f(lst->content));
		if (!current->next)
		{
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		else
			current = current->next;
	}
	return (new_list);
}
