/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"
#include "../../includes/builtins.h"
#include "../../includes/signals.h"
#include "../../includes/expander.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int	wait_child(int pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	exec_external(char **argv, t_env *env)
{
	char	*path;
	char	**envp;

	default_signals();
	path = resolve_path(argv[0], env);
	if (!path)
	{
		fprintf(stderr, "minishell: %s: command not found\n", argv[0]);
		exit(127);
	}
	envp = env_to_envp(env);
	if (!envp)
		exit(1);
	execve(path, argv, envp);
	perror("minishell");
	free(path);
	free_envp(envp);
	exit(126);
}

static int	exec_forked(char **argv, t_env *env)
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
		exec_external(argv, env);
	ignore_signals();
	status = wait_child(pid);
	init_signals();
	return (status);
}

static void	apply_wildcard_expansion(t_cmd_node *cmd, char ***old)
{
	char	**expanded;

	expanded = expand_argv_wildcards(cmd->argv);
	if (expanded)
	{
		*old = cmd->argv;
		cmd->argv = expanded;
	}
	else
		*old = NULL;
}

int	exec_command(t_cmd_node *cmd, t_exec_ctx *ctx)
{
	int		status;
	char	**old_argv;

	if (!cmd->argv || !cmd->argv[0])
		return (0);
	apply_wildcard_expansion(cmd, &old_argv);
	if (cmd->redirects)
	{
		if (process_all_heredocs(cmd->redirects, ctx) == -1)
			return (1);
		if (setup_redirects(cmd->redirects, ctx) == -1)
			return (1);
	}
	if (is_builtin(cmd->argv[0]))
		status = exec_builtin(cmd->argv, ctx);
	else
		status = exec_forked(cmd->argv, ctx->env);
	if (cmd->redirects)
		restore_redirects(ctx);
	if (old_argv)
		free_argv(old_argv);
	return (status);
}
