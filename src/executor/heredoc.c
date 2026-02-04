/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 15:30:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 15:30:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"
#include "../../includes/expander.h"
#include "../../includes/signals.h"
#include "../../libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int	write_to_pipe(int fd, char *content)
{
	size_t	len;

	if (!content)
		return (0);
	len = ft_strlen(content);
	if (write(fd, content, len) == -1)
		return (-1);
	if (write(fd, "\n", 1) == -1)
		return (-1);
	return (0);
}

static char	*read_heredoc_line(void)
{
	char	*line;
	size_t	len;

	line = NULL;
	len = 0;
	write(STDOUT_FILENO, "> ", 2);
	if (getline(&line, &len, stdin) == -1)
	{
		if (line)
			free(line);
		return (NULL);
	}
	if (g_sigint_received)
	{
		free(line);
		return (NULL);
	}
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}

static int	is_delimiter(char *line, char *delim)
{
	if (!line)
		return (0);
	return (ft_strcmp(line, delim) == 0);
}

static int	read_heredoc_content(int write_fd, char *delim, t_exec_ctx *ctx)
{
	char	*line;
	char	*expanded;

	heredoc_signals();
	while (1)
	{
		line = read_heredoc_line();
		if (g_sigint_received || !line)
		{
			if (line)
				free(line);
			init_signals();
			if (g_sigint_received)
				return (-1);
			break ;
		}
		if (is_delimiter(line, delim))
		{
			free(line);
			break ;
		}
		expanded = expand_variables(line, ctx);
		free(line);
		if (write_to_pipe(write_fd, expanded) == -1)
		{
			free(expanded);
			init_signals();
			return (-1);
		}
		free(expanded);
	}
	init_signals();
	return (0);
}

int	process_heredoc(t_redirect *redir, t_exec_ctx *ctx)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		return (-1);
	if (read_heredoc_content(pipefd[1], redir->file, ctx) == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	close(pipefd[1]);
	redir->heredoc_fd = pipefd[0];
	return (0);
}

int	process_all_heredocs(t_redirect *redir, t_exec_ctx *ctx)
{
	t_redirect	*curr;

	curr = redir;
	while (curr)
	{
		if (curr->type == REDIR_HEREDOC)
		{
			if (process_heredoc(curr, ctx) == -1)
				return (-1);
		}
		curr = curr->next;
	}
	return (0);
}
