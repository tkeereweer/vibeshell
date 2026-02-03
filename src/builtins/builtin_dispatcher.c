/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"
#include "../../libft/libft.h"

int	is_builtin(char *command)
{
	if (ft_strcmp(command, "echo") == 0)
		return (1);
	if (ft_strcmp(command, "cd") == 0)
		return (1);
	if (ft_strcmp(command, "pwd") == 0)
		return (1);
	if (ft_strcmp(command, "export") == 0)
		return (1);
	if (ft_strcmp(command, "unset") == 0)
		return (1);
	if (ft_strcmp(command, "env") == 0)
		return (1);
	if (ft_strcmp(command, "exit") == 0)
		return (1);
	return (0);
}

int	exec_builtin(char **argv, t_exec_ctx *ctx)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		return (builtin_echo(argv, ctx));
	if (ft_strcmp(argv[0], "cd") == 0)
		return (builtin_cd(argv, ctx));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (builtin_pwd(argv, ctx));
	if (ft_strcmp(argv[0], "export") == 0)
		return (builtin_export(argv, ctx));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (builtin_unset(argv, ctx));
	if (ft_strcmp(argv[0], "env") == 0)
		return (builtin_env(argv, ctx));
	if (ft_strcmp(argv[0], "exit") == 0)
		return (builtin_exit(argv, ctx));
	return (1);
}
