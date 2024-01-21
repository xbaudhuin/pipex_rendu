/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_pipex_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xabaudhu <xabaudhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:50:25 by xabaudhu          #+#    #+#             */
/*   Updated: 2024/01/19 16:04:03 by xabaudhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_line(char const *s, char c)
{
	size_t	line;
	size_t	i;
	int		flag;

	flag = 1;
	i = 0;
	line = 0;
	while (s[i])
	{
		if (s[i] != c && flag == 1)
		{
			line++;
			flag = 0;
		}
		if (s[i] == c && flag == 0)
			flag = 1;
		i++;
	}
	return (line);
}

static char	*ft_subpipex(char const *s, unsigned int start, size_t len, char c)
{
	char			*sub;
	unsigned int	i;
	size_t			len_s;

	if (!s)
		return (NULL);
	len_s = ft_strlen(s);
	if (start > len_s)
		len = 0;
	else if (len_s - start < len)
		len = len_s - start;
	sub = malloc(sizeof(*sub) * (len + 2));
	if (!sub)
		return (NULL);
	i = 0;
	while (start + i < len_s && s[start + i] && i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = c;
	sub[i + 1] = '\0';
	return (sub);
}

static char	**fill_split(char const *s, char c, char **split, size_t line)
{
	size_t	i;
	size_t	j;
	size_t	k;

	k = 0;
	i = 0;
	while (k < line)
	{
		while (s[i] && s[i] == c)
			i++;
		j = 0;
		while (s[i + j] && s[i + j] != c)
			j++;
		split[k] = ft_subpipex(s, i, j, '/');
		if (!split[k])
			return (free_split(split), NULL);
		i += j;
		k++;
	}
	split[k] = NULL;
	return (split);
}

char	**ft_split_pipex(char const *s, char c)
{
	size_t	line;
	char	**split;

	if (!s)
		return (NULL);
	line = count_line(s, c);
	split = malloc(sizeof(*split) * (line + 1));
	if (!split)
		return (NULL);
	split = fill_split(s, c, split, line);
	return (split);
}
