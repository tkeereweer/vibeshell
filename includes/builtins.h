/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "env.h"
# include "executor.h"

int		is_builtin(char *command);
int		exec_builtin(char **argv, struct s_exec_ctx *ctx);
int		builtin_echo(char **argv, struct s_exec_ctx *ctx);
int		builtin_cd(char **argv, struct s_exec_ctx *ctx);
int		builtin_pwd(char **argv, struct s_exec_ctx *ctx);
int		builtin_env(char **argv, struct s_exec_ctx *ctx);
int		builtin_export(char **argv, struct s_exec_ctx *ctx);
int		builtin_unset(char **argv, struct s_exec_ctx *ctx);
int		builtin_exit(char **argv, struct s_exec_ctx *ctx);

#endif
