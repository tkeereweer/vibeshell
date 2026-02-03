/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token_check.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_redir_token(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (1);
	if (type == TOKEN_REDIR_OUT || type == TOKEN_REDIR_APPEND)
		return (1);
	if (type == TOKEN_REDIR_HEREDOC)
		return (1);
	return (0);
}

int	is_operator_token(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return (1);
	if (type == TOKEN_AND || type == TOKEN_OR)
		return (1);
	return (0);
}

char	**alloc_argv_array(int count)
{
	char	**argv;

	argv = (char **)malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	argv[count] = NULL;
	return (argv);
}
