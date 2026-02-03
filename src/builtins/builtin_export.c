/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"
#include "../../libft/libft.h"
#include <stdio.h>

static void	print_all_vars(t_exec_ctx *ctx)
{
	t_env	*curr;

	curr = ctx->env;
	while (curr)
	{
		printf("declare -x %s=\"%s\"\n", curr->key, curr->value);
		curr = curr->next;
	}
}

static int	parse_export_arg(char *arg, char **key, char **value)
{
	int	i;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	if (arg[i] != '=')
		return (0);
	*key = (char *)malloc(i + 1);
	if (!*key)
		return (0);
	i = 0;
	while (arg[i] && arg[i] != '=')
	{
		(*key)[i] = arg[i];
		i++;
	}
	(*key)[i] = '\0';
	*value = ft_strdup(arg + i + 1);
	if (!*value)
	{
		free(*key);
		return (0);
	}
	return (1);
}

int	builtin_export(char **argv, t_exec_ctx *ctx)
{
	char	*key;
	char	*value;
	int		i;

	if (!argv[1])
	{
		print_all_vars(ctx);
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		if (parse_export_arg(argv[i], &key, &value))
		{
			env_set(&ctx->env, key, value);
			free(key);
			free(value);
		}
		i++;
	}
	return (0);
}
