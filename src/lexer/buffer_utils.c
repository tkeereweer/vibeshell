/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*buffer_extract(t_lexer *lex, int start, int end)
{
	char	*str;
	int		len;
	int		i;

	len = end - start;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = lex->input[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

void	buffer_add_word_token(t_lexer *lex)
{
	char	*value;
	t_token	*token;

	value = buffer_extract(lex, lex->start, lex->pos);
	if (!value)
		return ;
	token = token_create(TOKEN_WORD, value, lex->start);
	free(value);
	if (token)
		token_append(lex, token);
}
