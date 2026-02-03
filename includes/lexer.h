/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "tokens.h"

typedef enum e_lexer_state
{
	STATE_INITIAL,
	STATE_WORD,
	STATE_SQUOTE,
	STATE_DQUOTE,
	STATE_OPERATOR
}	t_lexer_state;

typedef struct s_lexer
{
	char			*input;
	int				pos;
	int				start;
	t_lexer_state	state;
	t_token			*tokens;
	t_token			*current;
}	t_lexer;

t_token			*tokenize(char *input);
void			lexer_init(t_lexer *lex, char *input);
void			lexer_process(t_lexer *lex);
void			lexer_free(t_lexer *lex);
void			handle_initial_state(t_lexer *lex);
void			handle_word_state(t_lexer *lex);
void			handle_squote_state(t_lexer *lex);
void			handle_dquote_state(t_lexer *lex);
void			handle_operator_state(t_lexer *lex);
int				is_whitespace(char c);
int				is_operator_char(char c);
int				is_quote(char c);
int				is_special_char(char c);
t_token_type	try_two_char_operator(char *input, int pos);
t_token_type	try_one_char_operator(char c);
t_token			*token_create(t_token_type type, char *value, int pos);
void			token_append(t_lexer *lex, t_token *token);
void			token_list_free(t_token *tokens);
char			*buffer_extract(t_lexer *lex, int start, int end);
void			buffer_add_word_token(t_lexer *lex);

#endif
