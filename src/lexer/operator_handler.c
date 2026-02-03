/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	add_operator_token(t_lexer *lex, t_token_type type, int len)
{
	char	*value;
	t_token	*token;

	value = buffer_extract(lex, lex->start, lex->start + len);
	if (!value)
		return ;
	token = token_create(type, value, lex->start);
	free(value);
	if (token)
		token_append(lex, token);
	lex->pos = lex->start + len;
	lex->state = STATE_INITIAL;
}

void	handle_operator_state(t_lexer *lex)
{
	t_token_type	type;

	type = try_two_char_operator(lex->input, lex->pos);
	if (type != TOKEN_EOF)
	{
		add_operator_token(lex, type, 2);
		return ;
	}
	type = try_one_char_operator(lex->input[lex->pos]);
	if (type != TOKEN_EOF)
	{
		add_operator_token(lex, type, 1);
		return ;
	}
	error_print("unknown operator");
	lex->pos++;
	lex->state = STATE_INITIAL;
}
