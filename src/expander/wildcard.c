/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 16:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/expander.h"
#include "../../libft/libft.h"
#include <dirent.h>
#include <stdlib.h>

static int	match_pattern(char *str, char *pattern)
{
	if (!*pattern)
		return (!*str);
	if (*pattern == '*')
	{
		if (match_pattern(str, pattern + 1))
			return (1);
		if (*str && match_pattern(str + 1, pattern))
			return (1);
		return (0);
	}
	if (*str == *pattern)
		return (match_pattern(str + 1, pattern + 1));
	return (0);
}

static int	should_match(char *filename, char *pattern)
{
	if (filename[0] == '.')
		return (0);
	return (match_pattern(filename, pattern));
}

static int	count_matches(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	count = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	entry = readdir(dir);
	while (entry)
	{
		if (should_match(entry->d_name, pattern))
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

static char	**collect_matches(char *pattern, int count)
{
	char			**result;
	DIR				*dir;
	struct dirent	*entry;
	int				i;

	result = (char **)malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	dir = opendir(".");
	if (!dir)
		return (free(result), NULL);
	i = 0;
	entry = readdir(dir);
	while (entry && i < count)
	{
		if (should_match(entry->d_name, pattern))
			result[i++] = ft_strdup(entry->d_name);
		entry = readdir(dir);
	}
	result[i] = NULL;
	closedir(dir);
	return (result);
}

char	**expand_wildcard(char *pattern)
{
	char	**matches;
	int		count;

	if (!pattern || !ft_strchr(pattern, '*'))
		return (NULL);
	count = count_matches(pattern);
	if (count == 0)
		return (NULL);
	matches = collect_matches(pattern, count);
	if (!matches)
		return (NULL);
	sort_matches(matches, count);
	return (matches);
}
