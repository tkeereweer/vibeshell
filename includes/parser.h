/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "tokens.h"
# include "ast.h"

typedef struct s_parse_ctx
{
	t_token	*curr;
	t_token	*tokens;
	int		error;
}	t_parse_ctx;

t_ast_node		*parse_tokens(t_token *tokens);
t_parse_ctx		*init_parse_ctx(t_token *tokens);
t_ast_node		*parse_or(t_parse_ctx *ctx);
t_ast_node		*parse_and(t_parse_ctx *ctx);
t_ast_node		*parse_pipe(t_parse_ctx *ctx);
t_ast_node		*parse_primary(t_parse_ctx *ctx);
t_ast_node		*parse_command(t_parse_ctx *ctx);
t_redirect		*parse_redirections(t_parse_ctx *ctx);
t_redirect		*parse_single_redir(t_parse_ctx *ctx);
t_redirect		*create_redirect(t_token_type type, char *file);
void			append_redirect(t_redirect **head, t_redirect *new);
int				is_redir_token(t_token_type type);
int				is_operator_token(t_token_type type);
int				match_token(t_parse_ctx *ctx, t_token_type type);
void			advance_token(t_parse_ctx *ctx);
t_token			*peek_token(t_parse_ctx *ctx);
int				validate_syntax(t_token *tokens);
int				check_operator_position(t_token *tok);
int				check_redirect_has_file(t_token *tok);
int				check_balanced_parens(t_token *tokens);
void			parse_error(char *msg, char *token);
void			syntax_error(char *token);
void			unexpected_eof_error(void);
void			unmatched_paren_error(void);
int				is_at_end(t_parse_ctx *ctx);
t_ast_node		*make_binop(t_node_type type, t_ast_node *l, t_ast_node *r);
void			cleanup_parse(t_parse_ctx *ctx, t_ast_node *node);
int				is_command_start(t_parse_ctx *ctx);
int				has_command_word(t_parse_ctx *ctx);
t_redir_type	token_to_redir(t_token_type type);
char			**build_argv(t_parse_ctx *ctx, int *count);
int				count_words(t_parse_ctx *ctx);
char			**alloc_argv_array(int count);
void			fill_argv_array(t_parse_ctx *ctx, char **argv, int count);

#endif
