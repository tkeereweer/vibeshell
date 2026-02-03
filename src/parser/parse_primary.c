/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_primary.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_ast_node	*parse_subshell(t_parse_ctx *ctx)
{
	t_ast_node	*child;
	t_ast_node	*node;
	t_redirect	*redir;

	advance_token(ctx);
	child = parse_or(ctx);
	if (!child || ctx->error)
		return (NULL);
	if (!match_token(ctx, TOKEN_RPAREN))
	{
		syntax_error(")");
		ctx->error = 1;
		free_ast(child);
		return (NULL);
	}
	redir = parse_redirections(ctx);
	node = create_subshell_node(child, redir);
	if (!node)
		ctx->error = 1;
	return (node);
}

t_ast_node	*parse_primary(t_parse_ctx *ctx)
{
	if (ctx->curr->type == TOKEN_LPAREN)
		return (parse_subshell(ctx));
	if (is_command_start(ctx))
		return (parse_command(ctx));
	syntax_error(ctx->curr->value);
	ctx->error = 1;
	return (NULL);
}
