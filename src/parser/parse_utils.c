/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_parse_ctx	*init_parse_ctx(t_token *tokens)
{
	t_parse_ctx	*ctx;

	ctx = (t_parse_ctx *)malloc(sizeof(t_parse_ctx));
	if (!ctx)
		return (NULL);
	ctx->curr = tokens;
	ctx->tokens = tokens;
	ctx->error = 0;
	return (ctx);
}

int	is_command_start(t_parse_ctx *ctx)
{
	if (!ctx->curr)
		return (0);
	if (ctx->curr->type == TOKEN_WORD)
		return (1);
	if (is_redir_token(ctx->curr->type))
		return (1);
	return (0);
}

int	has_command_word(t_parse_ctx *ctx)
{
	if (!ctx->curr)
		return (0);
	if (ctx->curr->type == TOKEN_WORD)
		return (1);
	return (0);
}

t_ast_node	*make_binop(t_node_type type, t_ast_node *l, t_ast_node *r)
{
	return (create_binop_node(type, l, r));
}

void	cleanup_parse(t_parse_ctx *ctx, t_ast_node *node)
{
	if (node)
		free_ast(node);
	if (ctx)
		free(ctx);
}
