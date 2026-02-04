/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"
#include "../../includes/signals.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

static void	exec_subshell_child(t_subshell_node *subshell, t_exec_ctx *ctx)
{
	int	status;

	default_signals();
	if (subshell->redirects)
	{
		process_all_heredocs(subshell->redirects, ctx);
		setup_redirects(subshell->redirects, ctx);
	}
	status = execute_ast(subshell->child, ctx);
	cleanup_exec_ctx(ctx);
	exit(status);
}

int	exec_subshell(t_subshell_node *subshell, t_exec_ctx *ctx)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
		exec_subshell_child(subshell, ctx);
	ignore_signals();
	status = wait_child(pid);
	init_signals();
	return (status);
}
