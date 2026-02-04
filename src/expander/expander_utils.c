/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 15:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 15:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/expander.h"
#include "../../libft/libft.h"
#include <stdlib.h>

char	*append_str(char *result, char *add)
{
	char	*new;

	if (!add)
		return (result);
	if (!result)
		return (ft_strdup(add));
	new = ft_strjoin(result, add);
	free(result);
	return (new);
}

char	*append_char(char *result, char c)
{
	char	temp[2];
	char	*new;

	temp[0] = c;
	temp[1] = '\0';
	if (!result)
		return (ft_strdup(temp));
	new = ft_strjoin(result, temp);
	free(result);
	return (new);
}

char	*append_result(char *result, char *add)
{
	char	*new;

	if (!add)
		return (result);
	if (!result)
		return (add);
	new = ft_strjoin(result, add);
	free(result);
	free(add);
	return (new);
}

char	*extract_substring(char *str, int start, int end)
{
	char	*result;
	int		len;

	len = end - start;
	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
	len = 0;
	while (start + len < end)
	{
		result[len] = str[start + len];
		len++;
	}
	result[len] = '\0';
	return (result);
}

char	*get_unquoted_segment(char *str, int *i)
{
	int	start;

	start = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '"')
		(*i)++;
	return (extract_substring(str, start, *i));
}
