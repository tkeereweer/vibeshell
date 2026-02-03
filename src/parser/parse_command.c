/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_cmd_end(t_parse_ctx *ctx)
{
	if (!ctx->curr)
		return (1);
	if (ctx->curr->type == TOKEN_EOF)
		return (1);
	if (ctx->curr->type == TOKEN_RPAREN)
		return (1);
	if (is_operator_token(ctx->curr->type))
		return (1);
	return (0);
}

static t_redirect	*parse_cmd_redirections(t_parse_ctx *ctx)
{
	t_redirect	*head;
	t_redirect	*new_redir;

	head = NULL;
	while (!is_cmd_end(ctx))
	{
		if (is_redir_token(ctx->curr->type))
		{
			new_redir = parse_single_redir(ctx);
			if (!new_redir)
				return (head);
			append_redirect(&head, new_redir);
		}
		else if (ctx->curr->type == TOKEN_WORD)
			break ;
		else
			advance_token(ctx);
	}
	return (head);
}

static char	**parse_cmd_words(t_parse_ctx *ctx, t_redirect **redir)
{
	char		**argv;
	t_redirect	*new_redir;
	int			count;

	argv = NULL;
	while (!is_cmd_end(ctx))
	{
		if (is_redir_token(ctx->curr->type))
		{
			new_redir = parse_single_redir(ctx);
			if (new_redir)
				append_redirect(redir, new_redir);
		}
		else if (ctx->curr->type == TOKEN_WORD && !argv)
			argv = build_argv(ctx, &count);
		else
			break ;
	}
	return (argv);
}

t_ast_node	*parse_command(t_parse_ctx *ctx)
{
	t_redirect	*redir;
	char		**argv;
	t_ast_node	*node;

	redir = parse_cmd_redirections(ctx);
	argv = parse_cmd_words(ctx, &redir);
	if (!argv && !redir)
	{
		ctx->error = 1;
		return (NULL);
	}
	node = create_cmd_node(argv, redir);
	if (!node)
	{
		ctx->error = 1;
		if (argv)
			free_argv(argv);
		if (redir)
			free_redirect_list(redir);
		return (NULL);
	}
	return (node);
}
