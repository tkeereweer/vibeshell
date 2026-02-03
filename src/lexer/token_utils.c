/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

t_token	*token_create(t_token_type type, char *value, int pos)
{
	t_token	*token;
	char	*val_copy;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	val_copy = NULL;
	if (value)
	{
		val_copy = ft_strdup(value);
		if (!val_copy)
		{
			free(token);
			return (NULL);
		}
	}
	token->type = type;
	token->value = val_copy;
	token->pos = pos;
	token->next = NULL;
	return (token);
}

void	token_append(t_lexer *lex, t_token *token)
{
	if (!lex->tokens)
	{
		lex->tokens = token;
		lex->current = token;
	}
	else
	{
		lex->current->next = token;
		lex->current = token;
	}
}

void	token_list_free(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = temp;
	}
}

void	free_tokens(t_token *tokens)
{
	token_list_free(tokens);
}
