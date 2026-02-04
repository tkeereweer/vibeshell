/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 16:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 16:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/expander.h"
#include "../../libft/libft.h"
#include <stdlib.h>

void	sort_matches(char **matches, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(matches[i], matches[j]) > 0)
			{
				temp = matches[i];
				matches[i] = matches[j];
				matches[j] = temp;
			}
			j++;
		}
		i++;
	}
}

int	count_expanded_args(char **argv)
{
	int		total;
	int		i;
	int		j;
	char	**matches;

	total = 0;
	i = 0;
	while (argv[i])
	{
		matches = expand_wildcard(argv[i]);
		if (matches)
		{
			j = 0;
			while (matches[j])
			{
				total++;
				j++;
			}
			ft_free_split(matches);
		}
		else
			total++;
		i++;
	}
	return (total);
}

void	copy_matches(char **result, char **matches, int *idx)
{
	int	i;

	i = 0;
	while (matches[i])
	{
		result[*idx] = matches[i];
		(*idx)++;
		i++;
	}
	free(matches);
}

char	**expand_argv_wildcards(char **argv)
{
	char	**result;
	char	**matches;
	int		total;
	int		i;
	int		j;

	if (!argv)
		return (NULL);
	total = count_expanded_args(argv);
	result = (char **)malloc(sizeof(char *) * (total + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (argv[i])
	{
		matches = expand_wildcard(argv[i]);
		if (matches)
			copy_matches(result, matches, &j);
		else
			result[j++] = ft_strdup(argv[i]);
		i++;
	}
	result[j] = NULL;
	return (result);
}
