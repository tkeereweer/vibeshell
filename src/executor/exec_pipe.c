/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

static void	exec_pipe_child(t_ast_node *node, t_exec_ctx *ctx)
{
	int	status;

	status = execute_ast(node, ctx);
	cleanup_exec_ctx(ctx);
	exit(status);
}

static void	setup_pipe_fds(int in_fd, int out_fd)
{
	if (in_fd != -1)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != -1)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
}

static int	fork_pipe_cmd(t_ast_node *node, int fds[2], t_exec_ctx *ctx)
{
	int	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (-1);
	}
	if (pid == 0)
	{
		setup_pipe_fds(fds[0], fds[1]);
		exec_pipe_child(node, ctx);
	}
	return (pid);
}

static int	wait_pipe_children(int pid_left, int pid_right)
{
	int	status;

	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	exec_pipe(t_binop_node *pipe_node, t_exec_ctx *ctx)
{
	int		pipefd[2];
	int		fds[2];
	int		pid[2];

	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe"), 1);
	fds[0] = -1;
	fds[1] = pipefd[1];
	pid[0] = fork_pipe_cmd(pipe_node->left, fds, ctx);
	close(pipefd[1]);
	if (pid[0] == -1)
		return (close(pipefd[0]), 1);
	fds[0] = pipefd[0];
	fds[1] = -1;
	pid[1] = fork_pipe_cmd(pipe_node->right, fds, ctx);
	close(pipefd[0]);
	if (pid[1] == -1)
		return (1);
	return (wait_pipe_children(pid[0], pid[1]));
}
