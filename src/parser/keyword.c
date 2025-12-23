/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyword.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beatde-a <beatde-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:30:31 by beatde-a          #+#    #+#             */
/*   Updated: 2025/12/23 11:43:04 by beatde-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	check_keyword_acl(const char *p, char *key, t_metadata *meta)
{
	if (p[0] == 'A' && (ft_isspace((unsigned char)p[1]) || p[1] == '#'))
	{
		ft_strlcpy(key, "A", 4);
		meta->ambient_count++;
		return (1);
	}
	else if (p[0] == 'C' && (ft_isspace((unsigned char)p[1]) || p[1] == '#'))
	{
		ft_strlcpy(key, "C", 4);
		meta->camera_count++;
		return (1);
	}
	else if (p[0] == 'L' && (ft_isspace((unsigned char)p[1]) || p[1] == '#'))
	{
		ft_strlcpy(key, "L", 4);
		meta->light_count++;
		return (1);
	}
	return (0);
}

static int	check_keyword_object(const char *p, char *key)
{
	if (!ft_strncmp(p, "sp", 2)
		&& (ft_isspace((unsigned char)p[2]) || p[2] == '#'))
	{
		ft_strlcpy(key, "sp", 4);
		return (1);
	}
	else if (!ft_strncmp(p, "pl", 2)
		&& (ft_isspace((unsigned char)p[2]) || p[2] == '#'))
	{
		ft_strlcpy(key, "pl", 4);
		return (1);
	}
	else if (!ft_strncmp(p, "cy", 2)
		&& (ft_isspace((unsigned char)p[2]) || p[2] == '#'))
	{
		ft_strlcpy(key, "cy", 4);
		return (1);
	}
	else if (!ft_strncmp(p, "co", 2)
		&& (ft_isspace((unsigned char)p[2]) || p[2] == '#'))
	{
		ft_strlcpy(key, "co", 4);
		return (1);
	}
	return (0);
}

int	is_valid_keyword(const char *p, char *key, t_metadata *meta)
{
	key[0] = '\0';
	if (check_keyword_acl(p, key, meta))
		return (1);
	if (check_keyword_object(p, key))
		return (1);
	return (0);
}
