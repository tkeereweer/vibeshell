/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
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

static char	*process_squote(char *str, int *i)
{
	char	*result;
	int		start;

	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	result = extract_substring(str, start, *i);
	if (str[*i] == '\'')
		(*i)++;
	return (result);
}

static char	*process_dquote(char *str, int *i, t_exec_ctx *ctx)
{
	char	*temp;
	char	*result;
	int		start;

	(*i)++;
	start = *i;
	while (str[*i] && str[*i] != '"')
		(*i)++;
	temp = extract_substring(str, start, *i);
	if (!temp)
		return (NULL);
	result = expand_variables(temp, ctx);
	free(temp);
	if (str[*i] == '"')
		(*i)++;
	return (result);
}

static char	*process_unquoted(char *str, int *i, t_exec_ctx *ctx)
{
	char	*temp;
	char	*expanded;

	temp = get_unquoted_segment(str, i);
	expanded = expand_variables(temp, ctx);
	free(temp);
	return (expanded);
}

static char	*process_segment(char *str, int *i, char **r, t_exec_ctx *ctx)
{
	char	*temp;

	if (str[*i] == '\'')
	{
		temp = process_squote(str, i);
		*r = append_result(*r, temp);
	}
	else if (str[*i] == '"')
	{
		temp = process_dquote(str, i, ctx);
		*r = append_result(*r, temp);
	}
	else
	{
		temp = process_unquoted(str, i, ctx);
		*r = append_result(*r, temp);
	}
	return (*r);
}

char	*remove_quotes(char *str, t_exec_ctx *ctx)
{
	char	*result;
	int		i;

	result = NULL;
	i = 0;
	while (str[i])
		process_segment(str, &i, &result, ctx);
	if (!result)
		return (ft_strdup(""));
	return (result);
}
