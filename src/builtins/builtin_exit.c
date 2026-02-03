/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"
#include <stdio.h>
#include <stdlib.h>

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	parse_exit_code(char *str)
{
	int		code;
	int		sign;
	int		i;

	code = 0;
	sign = 1;
	i = 0;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i])
	{
		code = code * 10 + (str[i] - '0');
		i++;
	}
	return ((code * sign) % 256);
}

int	builtin_exit(char **argv, t_exec_ctx *ctx)
{
	int	code;

	printf("exit\n");
	if (!argv[1])
		exit(ctx->last_status);
	if (!is_numeric(argv[1]))
	{
		fprintf(stderr, "minishell: exit: %s: ", argv[1]);
		fprintf(stderr, "numeric argument required\n");
		exit(2);
	}
	code = parse_exit_code(argv[1]);
	if (code < 0)
		code = 256 + code;
	exit(code);
}
