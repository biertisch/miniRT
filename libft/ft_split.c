/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:36:31 by bliu              #+#    #+#             */
/*   Updated: 2025/06/12 20:15:38 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Allocates memory (using malloc(3)) and returns an array of strings
 *        obtained by splitting the string s using the char c as a delimiter.
 *        The array must be NULL-terminated.
 *
 * @param s The string to be split.
 * @param c The delimiter character.
 * @return The array of new strings resulting from the split.
 *         NULL if the allocation fails.
 */

static int	count_str(char const *s, char c)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] && s[i] != c)
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

static int	assign_str(char **result, char *str, int j)
{
	result[j] = str;
	if (result[j] == NULL)
	{
		while (j-- > 0)
			free(result[j]);
		free(result);
		result = NULL;
		return (0);
	}
	return (1);
}

static void	assign_array(char const *s, char c, char **result)
{
	int		i;
	int		j;
	int		start;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		if (i > start)
		{
			if (!assign_str(result, ft_substr(s, start, i - start), j))
				return ;
			j++;
		}
	}
	result[j] = NULL;
}

char	**ft_split(char const *s, char c)
{
	char	**result;

	if (s == NULL)
		return (NULL);
	result = malloc((count_str(s, c) + 1) * sizeof(char *));
	if (result == NULL)
		return (NULL);
	assign_array(s, c, result);
	return (result);
}
