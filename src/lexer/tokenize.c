/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	lexer_init(t_lexer *lex, char *input)
{
	lex->input = input;
	lex->pos = 0;
	lex->start = 0;
	lex->state = STATE_INITIAL;
	lex->tokens = NULL;
	lex->current = NULL;
}

void	lexer_free(t_lexer *lex)
{
	if (lex->tokens)
		token_list_free(lex->tokens);
	lex->tokens = NULL;
	lex->current = NULL;
}

t_token	*tokenize(char *input)
{
	t_lexer	lex;

	lexer_init(&lex, input);
	lexer_process(&lex);
	return (lex.tokens);
}
