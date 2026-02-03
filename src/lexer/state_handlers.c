/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_initial_state(t_lexer *lex)
{
	char	c;

	c = lex->input[lex->pos];
	if (is_whitespace(c))
	{
		lex->pos++;
		return ;
	}
	lex->start = lex->pos;
	if (is_quote(c))
	{
		if (c == '\'')
			lex->state = STATE_SQUOTE;
		else
			lex->state = STATE_DQUOTE;
		lex->pos++;
	}
	else if (is_operator_char(c))
		lex->state = STATE_OPERATOR;
	else
		lex->state = STATE_WORD;
}

void	handle_word_state(t_lexer *lex)
{
	char	c;

	c = lex->input[lex->pos];
	if (c == '\0' || is_whitespace(c) || is_special_char(c))
	{
		buffer_add_word_token(lex);
		lex->state = STATE_INITIAL;
		return ;
	}
	lex->pos++;
}
