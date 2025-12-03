/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bliu <bliu@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 12:47:40 by bliu              #+#    #+#             */
/*   Updated: 2025/06/17 19:13:34 by bliu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	count_max_args(const char *s, int *count, int i, int in_quote)
{
	char	q;

	q = '\0';
	while (s[i])
	{
		while (ft_isspace(s[i]))
			i++;
		if (s[i])
			*count = *count + 1;
		while (s[i] && (!ft_isspace(s[i]) || in_quote))
		{
			if ((s[i] == '\'' || s[i] == '"'))
			{
				if (in_quote && s[i] == q)
					in_quote = 0;
				else if (!in_quote)
				{
					in_quote = 1;
					q = s[i];
				}
			}
			i++;
		}
	}
}

static int	handle_quotes(char *buf, int *i, int *in_quote, char *q)
{
	if (*in_quote && buf[*i] == *q)
	{
		*in_quote = 0;
		ft_memmove(buf + *i, buf + *i + 1, ft_strlen(buf + *i));
		return (1);
	}
	else if (!*in_quote)
	{
		*in_quote = 1;
		*q = buf[*i];
		ft_memmove(buf + *i, buf + *i + 1, ft_strlen(buf + *i));
		return (1);
	}
	return (0);
}

static void	rep_delimiter_rmv_quotes(char *buf)
{
	int		i;
	int		in_quote;
	char	q;

	i = 0;
	in_quote = 0;
	q = '\0';
	while (buf[i])
	{
		if (buf[i] == '"' || buf[i] == '\'')
		{
			if (handle_quotes(buf, &i, &in_quote, &q))
				continue ;
		}
		else if (ft_isspace(buf[i]) && !in_quote)
			buf[i] = '\0';
		i++;
	}
}

static void	fill_argv(char **argv, char *buf, int len)
{
	int	i;
	int	argc;

	i = 0;
	argc = 0;
	while (i < len)
	{
		while (i < len && buf[i] == '\0')
			i++;
		if (i < len)
		{
			argv[argc++] = ft_strdup(&buf[i]);
			while (i < len && buf[i] != '\0')
				i++;
		}
	}
	argv[argc] = NULL;
}

char	**ft_split_cmd(const char *cmdstr)
{
	char	*buf;
	char	**argv;
	int		max_args;
	int		len;

	if (!cmdstr)
		return (NULL);
	buf = ft_strdup(cmdstr);
	if (!buf)
		return (NULL);
	max_args = 0;
	count_max_args(buf, &max_args, 0, 0);
	argv = malloc(sizeof(char *) * (max_args + 1));
	if (!argv)
	{
		free(buf);
		return (NULL);
	}
	rep_delimiter_rmv_quotes(buf);
	len = ft_strlen(cmdstr);
	fill_argv(argv, buf, len);
	free(buf);
	return (argv);
}

/*
void	test_ft_split_command(char *cmd)
{
	char	**argv = ft_split_command(cmd);
	int		i = 0;
	
	printf("\ncommand id:[%s]\n", cmd);
	while (argv && argv[i])
	{
		printf("argv[%d] = [%s]\n", i, argv[i]);
		i++;
	}
	ft_free_array(argv);
}

int	main(void)
{
	char	*cmd = "   awk '{count++} END {printf \"count: %i\" , count}'";
	test_ft_split_command(cmd);
	
	cmd = "' echo' \"Hello, World!\"";
	test_ft_split_command(cmd);
	
	cmd = "   echo 'Hello, World!'";
	test_ft_split_command(cmd);

	cmd = "   echo \"Hello, World!\" | grep 'World'";
	test_ft_split_command(cmd);
	
	cmd = "   echo \"Hello, World!\" | grep 'World' | awk -F ',' '{print $1}'";
	test_ft_split_command(cmd);
	
	cmd = "'echo' \"'Hello, World!'\"";
	test_ft_split_command(cmd);
	

	cmd = "'echo' '' ' ' '  ' '\"\"' \"''\"  \"'\"  '\"' \"'Hello, World!'\"";
	test_ft_split_command(cmd);
	

	cmd = "";
	test_ft_split_command(cmd);


	cmd = "''";
	test_ft_split_command(cmd);

	cmd = NULL;
	test_ft_split_command(cmd);


	return (0);
}
*/
