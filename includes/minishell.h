/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "tokens.h"
# include "lexer.h"
# include "ast.h"
# include "parser.h"
# include <stdlib.h>
# include <unistd.h>

# define ERR_UNCLOSED_QUOTE 1
# define ERR_MALLOC 2

void	error_exit(char *msg, int code);
void	error_print(char *msg);

#endif
