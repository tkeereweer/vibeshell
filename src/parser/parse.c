/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_ast_node	*parse_with_context(t_parse_ctx *ctx)
{
	t_ast_node	*ast;

	if (is_at_end(ctx))
		return (NULL);
	ast = parse_or(ctx);
	if (ctx->error || !ast)
		return (NULL);
	if (!is_at_end(ctx))
	{
		syntax_error(ctx->curr->value);
		free_ast(ast);
		return (NULL);
	}
	return (ast);
}

t_ast_node	*parse_tokens(t_token *tokens)
{
	t_parse_ctx	*ctx;
	t_ast_node	*ast;

	if (!tokens)
		return (NULL);
	ctx = init_parse_ctx(tokens);
	if (!ctx)
		return (NULL);
	ast = parse_with_context(ctx);
	if (!ast)
	{
		free(ctx);
		return (NULL);
	}
	free(ctx);
	return (ast);
}
