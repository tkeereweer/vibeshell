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
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

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
	{
		if (subshell->redirects)
			setup_redirects(subshell->redirects, ctx);
		status = execute_ast(subshell->child, ctx);
		cleanup_exec_ctx(ctx);
		exit(status);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
