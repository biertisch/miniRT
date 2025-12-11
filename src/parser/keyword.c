/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyword.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:30:31 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/11 15:37:12 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	check_keyword_acl(const char *p, char *key, t_color *rgb_acl)
{
	if (ft_strncmp(p, "A ", 2) == 0)
	{
		ft_strlcpy(key, "A", 4);
		rgb_acl->r++;
		return (1);
	}
	else if (ft_strncmp(p, "C ", 2) == 0)
	{
		ft_strlcpy(key, "C", 4);
		rgb_acl->g++;
		return (1);
	}
	else if (ft_strncmp(p, "L ", 2) == 0)
	{
		ft_strlcpy(key, "L", 4);
		rgb_acl->b++;
		return (1);
	}
	return (0);
}

static int	check_keyword_object(const char *p, char *key)
{
	if (ft_strncmp(p, "sp ", 3) == 0)
	{
		ft_strlcpy(key, "sp", 4);
		return (1);
	}
	else if (ft_strncmp(p, "pl ", 3) == 0)
	{
		ft_strlcpy(key, "pl", 4);
		return (1);
	}
	else if (ft_strncmp(p, "cy ", 3) == 0)
	{
		ft_strlcpy(key, "cy", 4);
		return (1);
	}
	else if (ft_strncmp(p, "co ", 3) == 0)
	{
		ft_strlcpy(key, "co", 4);
		return (1);
	}
	return (0);
}

int	is_valid_keyword(const char *p, char *key, t_color *acl)
{
	key[0] = '\0';
	if (check_keyword_acl(p, key, acl))
		return (1);
	if (check_keyword_object(p, key))
		return (1);
	return (0);
}
