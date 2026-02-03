/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "ast.h"
# include "env.h"

typedef struct s_exec_ctx
{
	t_env	*env;
	int		last_status;
	int		stdin_backup;
	int		stdout_backup;
}	t_exec_ctx;

int			execute_ast(t_ast_node *ast, t_exec_ctx *ctx);
int			exec_command(t_cmd_node *cmd, t_exec_ctx *ctx);
int			exec_pipe(t_binop_node *pipe, t_exec_ctx *ctx);
int			exec_and(t_binop_node *and_node, t_exec_ctx *ctx);
int			exec_or(t_binop_node *or_node, t_exec_ctx *ctx);
int			exec_subshell(t_subshell_node *subshell, t_exec_ctx *ctx);
char		*resolve_path(char *command, t_env *env);
char		**env_to_envp(t_env *env);
void		free_envp(char **envp);
t_exec_ctx	*init_exec_ctx(char **environ);
void		cleanup_exec_ctx(t_exec_ctx *ctx);
int			exec_external(char **argv, t_env *env);
int			wait_child(int pid);
int			setup_redirects(t_redirect *redir, t_exec_ctx *ctx);
void		restore_redirects(t_exec_ctx *ctx);

#endif
