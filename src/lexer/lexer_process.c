/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	dispatch_state(t_lexer *lex)
{
	if (lex->state == STATE_INITIAL)
		handle_initial_state(lex);
	else if (lex->state == STATE_WORD)
		handle_word_state(lex);
	else if (lex->state == STATE_SQUOTE)
		handle_squote_state(lex);
	else if (lex->state == STATE_DQUOTE)
		handle_dquote_state(lex);
	else if (lex->state == STATE_OPERATOR)
		handle_operator_state(lex);
}

void	lexer_process(t_lexer *lex)
{
	t_token	*eof_token;

	while (lex->input[lex->pos] != '\0')
		dispatch_state(lex);
	if (lex->state == STATE_WORD)
		buffer_add_word_token(lex);
	eof_token = token_create(TOKEN_EOF, NULL, lex->pos);
	if (eof_token)
		token_append(lex, eof_token);
}
