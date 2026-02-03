/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void	advance_token(t_parse_ctx *ctx)
{
	if (ctx->curr && ctx->curr->next)
		ctx->curr = ctx->curr->next;
}

int	match_token(t_parse_ctx *ctx, t_token_type type)
{
	if (ctx->curr->type == type)
	{
		advance_token(ctx);
		return (1);
	}
	return (0);
}

t_token	*peek_token(t_parse_ctx *ctx)
{
	return (ctx->curr);
}

int	is_at_end(t_parse_ctx *ctx)
{
	if (!ctx->curr || ctx->curr->type == TOKEN_EOF)
		return (1);
	return (0);
}

int	count_words(struct s_parse_ctx *ctx)
{
	int		count;
	t_token	*temp;

	count = 0;
	temp = ctx->curr;
	while (temp && temp->type == TOKEN_WORD)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}
