/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 15:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 15:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/expander.h"
#include <stdlib.h>

void	expand_tokens(t_token *tokens, t_exec_ctx *ctx)
{
	t_token	*curr;
	char	*expanded;

	curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_WORD && curr->value)
		{
			expanded = remove_quotes(curr->value, ctx);
			if (expanded)
			{
				free(curr->value);
				curr->value = expanded;
			}
		}
		curr = curr->next;
	}
}
