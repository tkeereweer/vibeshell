/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_operator_position(t_token *tok)
{
	if (!tok || !tok->next)
		return (0);
	if (is_operator_token(tok->type))
	{
		if (!tok->next || tok->next->type == TOKEN_EOF)
			return (0);
		if (is_operator_token(tok->next->type))
			return (0);
	}
	return (1);
}

int	check_redirect_has_file(t_token *tok)
{
	if (!tok || !tok->next)
		return (0);
	if (is_redir_token(tok->type))
	{
		if (!tok->next || tok->next->type != TOKEN_WORD)
			return (0);
	}
	return (1);
}

int	check_balanced_parens(t_token *tokens)
{
	int		count;
	t_token	*tok;

	count = 0;
	tok = tokens;
	while (tok && tok->type != TOKEN_EOF)
	{
		if (tok->type == TOKEN_LPAREN)
			count++;
		else if (tok->type == TOKEN_RPAREN)
			count--;
		if (count < 0)
			return (0);
		tok = tok->next;
	}
	if (count != 0)
		return (0);
	return (1);
}

int	validate_syntax(t_token *tokens)
{
	t_token	*tok;

	if (!tokens)
		return (0);
	if (is_operator_token(tokens->type))
		return (0);
	if (!check_balanced_parens(tokens))
		return (0);
	tok = tokens;
	while (tok && tok->type != TOKEN_EOF)
	{
		if (!check_operator_position(tok))
			return (0);
		if (!check_redirect_has_file(tok))
			return (0);
		tok = tok->next;
	}
	return (1);
}
