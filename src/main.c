/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

extern char	**environ;

static void	process_line(char *line, t_exec_ctx *ctx)
{
	t_token		*tokens;
	t_ast_node	*ast;
	int			status;

	if (!line || line[0] == '\0')
		return ;
	tokens = tokenize(line);
	if (!tokens)
		return ;
	if (!validate_syntax(tokens))
	{
		free_tokens(tokens);
		return ;
	}
	ast = parse_tokens(tokens);
	if (!ast)
	{
		free_tokens(tokens);
		return ;
	}
	status = execute_ast(ast, ctx);
	ctx->last_status = status;
	free_ast(ast);
	free_tokens(tokens);
}

static void	shell_loop(t_exec_ctx *ctx)
{
	char	*line;

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (line[0] != '\0')
			add_history(line);
		process_line(line, ctx);
		free(line);
	}
}

int	main(void)
{
	t_exec_ctx	*ctx;

	ctx = init_exec_ctx(environ);
	if (!ctx)
	{
		fprintf(stderr, "minishell: failed to initialize\n");
		return (1);
	}
	shell_loop(ctx);
	cleanup_exec_ctx(ctx);
	return (0);
}
