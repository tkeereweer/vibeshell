/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

static int	open_redir_file(t_redirect *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == REDIR_IN)
		fd = open(redir->file, O_RDONLY);
	else if (redir->type == REDIR_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->type == REDIR_HEREDOC)
		fd = redir->heredoc_fd;
	if (fd == -1 && redir->type != REDIR_HEREDOC)
		perror("minishell");
	return (fd);
}

static int	apply_single_redir(t_redirect *redir)
{
	int	fd;
	int	target_fd;

	fd = open_redir_file(redir);
	if (fd == -1)
		return (-1);
	target_fd = STDOUT_FILENO;
	if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
		target_fd = STDIN_FILENO;
	if (dup2(fd, target_fd) == -1)
	{
		close(fd);
		perror("minishell: dup2");
		return (-1);
	}
	close(fd);
	return (0);
}

int	setup_redirects(t_redirect *redir, t_exec_ctx *ctx)
{
	t_redirect	*curr;

	ctx->stdin_backup = dup(STDIN_FILENO);
	ctx->stdout_backup = dup(STDOUT_FILENO);
	if (ctx->stdin_backup == -1 || ctx->stdout_backup == -1)
		return (-1);
	curr = redir;
	while (curr)
	{
		if (apply_single_redir(curr) == -1)
			return (-1);
		curr = curr->next;
	}
	return (0);
}

void	restore_redirects(t_exec_ctx *ctx)
{
	if (ctx->stdin_backup != -1)
	{
		dup2(ctx->stdin_backup, STDIN_FILENO);
		close(ctx->stdin_backup);
		ctx->stdin_backup = -1;
	}
	if (ctx->stdout_backup != -1)
	{
		dup2(ctx->stdout_backup, STDOUT_FILENO);
		close(ctx->stdout_backup);
		ctx->stdout_backup = -1;
	}
}
