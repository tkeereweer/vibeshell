/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
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

static char	*get_var_name(char *str, int *len)
{
	int		i;
	char	*name;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*len = i;
	name = (char *)malloc(i + 1);
	if (!name)
		return (NULL);
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		name[i] = str[i];
		i++;
	}
	name[i] = '\0';
	return (name);
}

static char	*get_var_value(char *name, t_exec_ctx *ctx)
{
	char	*value;

	if (!name || !name[0])
		return (ft_strdup(""));
	value = env_get(ctx->env, name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

static char	*expand_status(t_exec_ctx *ctx)
{
	char	*status_str;
	int		status;
	int		len;
	int		temp;

	status = ctx->last_status;
	temp = status;
	len = 1;
	while (temp >= 10)
	{
		temp /= 10;
		len++;
	}
	status_str = (char *)malloc(len + 1);
	if (!status_str)
		return (NULL);
	status_str[len] = '\0';
	while (len > 0)
	{
		status_str[--len] = (status % 10) + '0';
		status /= 10;
	}
	return (status_str);
}

static int	process_dollar(char *str, int i, char **r, t_exec_ctx *ctx)
{
	char	*var_name;
	char	*var_value;
	int		var_len;

	if (str[i + 1] == '?')
	{
		var_value = expand_status(ctx);
		*r = append_str(*r, var_value);
		free(var_value);
		return (2);
	}
	else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
	{
		var_name = get_var_name(&str[i + 1], &var_len);
		var_value = get_var_value(var_name, ctx);
		*r = append_str(*r, var_value);
		free(var_name);
		free(var_value);
		return (var_len + 1);
	}
	*r = append_str(*r, "$");
	return (1);
}

char	*expand_variables(char *str, t_exec_ctx *ctx)
{
	char	*result;
	int		i;

	result = NULL;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
			i += process_dollar(str, i, &result, ctx);
		else
		{
			result = append_char(result, str[i]);
			i++;
		}
	}
	if (!result)
		return (ft_strdup(""));
	return (result);
}
