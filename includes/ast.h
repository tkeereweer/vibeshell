/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include <stdlib.h>
# include "tokens.h"

typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}	t_node_type;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redirect
{
	t_redir_type		type;
	char				*file;
	int					heredoc_fd;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_cmd_node
{
	char		**argv;
	t_redirect	*redirects;
}	t_cmd_node;

typedef struct s_binop_node
{
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_binop_node;

typedef struct s_subshell_node
{
	struct s_ast_node	*child;
	t_redirect			*redirects;
}	t_subshell_node;

typedef union u_node_data
{
	t_cmd_node			cmd;
	t_binop_node		binop;
	t_subshell_node		subshell;
}	t_node_data;

typedef struct s_ast_node
{
	t_node_type	type;
	t_node_data	data;
}	t_ast_node;

t_ast_node		*create_cmd_node(char **argv, t_redirect *redir);
t_ast_node		*create_binop_node(t_node_type type,
					t_ast_node *left, t_ast_node *right);
t_ast_node		*create_subshell_node(t_ast_node *child, t_redirect *redir);
void			free_ast(t_ast_node *node);
void			free_redirect_list(t_redirect *redir);
void			free_argv(char **argv);

#endif
