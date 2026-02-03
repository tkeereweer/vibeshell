/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_squote_state(t_lexer *lex)
{
	char	c;

	c = lex->input[lex->pos];
	if (c == '\0')
	{
		error_print("unclosed single quote");
		return ;
	}
	if (c == '\'')
	{
		lex->pos++;
		buffer_add_word_token(lex);
		lex->state = STATE_INITIAL;
		return ;
	}
	lex->pos++;
}

void	handle_dquote_state(t_lexer *lex)
{
	char	c;

	c = lex->input[lex->pos];
	if (c == '\0')
	{
		error_print("unclosed double quote");
		return ;
	}
	if (c == '\"')
	{
		lex->pos++;
		buffer_add_word_token(lex);
		lex->state = STATE_INITIAL;
		return ;
	}
	lex->pos++;
}
