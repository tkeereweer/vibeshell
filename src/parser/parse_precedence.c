/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_precedence.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast_node	*parse_pipe(t_parse_ctx *ctx)
{
	t_ast_node	*left;
	t_ast_node	*right;

	left = parse_primary(ctx);
	if (!left || ctx->error)
		return (left);
	while (match_token(ctx, TOKEN_PIPE))
	{
		right = parse_primary(ctx);
		if (!right || ctx->error)
		{
			free_ast(left);
			return (NULL);
		}
		left = make_binop(NODE_PIPE, left, right);
		if (!left)
		{
			ctx->error = 1;
			return (NULL);
		}
	}
	return (left);
}

t_ast_node	*parse_and(t_parse_ctx *ctx)
{
	t_ast_node	*left;
	t_ast_node	*right;

	left = parse_pipe(ctx);
	if (!left || ctx->error)
		return (left);
	while (match_token(ctx, TOKEN_AND))
	{
		right = parse_pipe(ctx);
		if (!right || ctx->error)
		{
			free_ast(left);
			return (NULL);
		}
		left = make_binop(NODE_AND, left, right);
		if (!left)
		{
			ctx->error = 1;
			return (NULL);
		}
	}
	return (left);
}

t_ast_node	*parse_or(t_parse_ctx *ctx)
{
	t_ast_node	*left;
	t_ast_node	*right;

	left = parse_and(ctx);
	if (!left || ctx->error)
		return (left);
	while (match_token(ctx, TOKEN_OR))
	{
		right = parse_and(ctx);
		if (!right || ctx->error)
		{
			free_ast(left);
			return (NULL);
		}
		left = make_binop(NODE_OR, left, right);
		if (!left)
		{
			ctx->error = 1;
			return (NULL);
		}
	}
	return (left);
}
