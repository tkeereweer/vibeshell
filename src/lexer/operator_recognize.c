/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_recognize.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token_type	try_two_char_operator(char *input, int pos)
{
	char	c1;
	char	c2;

	c1 = input[pos];
	c2 = input[pos + 1];
	if (c1 == '<' && c2 == '<')
		return (TOKEN_REDIR_HEREDOC);
	if (c1 == '>' && c2 == '>')
		return (TOKEN_REDIR_APPEND);
	if (c1 == '&' && c2 == '&')
		return (TOKEN_AND);
	if (c1 == '|' && c2 == '|')
		return (TOKEN_OR);
	return (TOKEN_EOF);
}

t_token_type	try_one_char_operator(char c)
{
	if (c == '|')
		return (TOKEN_PIPE);
	if (c == '<')
		return (TOKEN_REDIR_IN);
	if (c == '>')
		return (TOKEN_REDIR_OUT);
	if (c == '(')
		return (TOKEN_LPAREN);
	if (c == ')')
		return (TOKEN_RPAREN);
	return (TOKEN_EOF);
}
