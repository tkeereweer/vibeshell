/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 15:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 15:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "executor.h"

char	*expand_variables(char *str, t_exec_ctx *ctx);
char	*remove_quotes(char *str, t_exec_ctx *ctx);
void	expand_tokens(t_token *tokens, t_exec_ctx *ctx);
char	*append_str(char *result, char *add);
char	*append_char(char *result, char c);
char	*append_result(char *result, char *add);
char	*extract_substring(char *str, int start, int end);
char	*get_unquoted_segment(char *str, int *i);
char	**expand_wildcard(char *pattern);
char	**expand_argv_wildcards(char **argv);
int		count_expanded_args(char **argv);
void	copy_matches(char **result, char **matches, int *idx);
void	sort_matches(char **matches, int count);

#endif
