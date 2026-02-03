/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"
#include "../../includes/builtins.h"

int	execute_ast(t_ast_node *ast, t_exec_ctx *ctx)
{
	int	status;

	if (!ast)
		return (0);
	status = 0;
	if (ast->type == NODE_COMMAND)
		status = exec_command(&ast->data.cmd, ctx);
	else if (ast->type == NODE_PIPE)
		status = exec_pipe(&ast->data.binop, ctx);
	else if (ast->type == NODE_AND)
		status = exec_and(&ast->data.binop, ctx);
	else if (ast->type == NODE_OR)
		status = exec_or(&ast->data.binop, ctx);
	else if (ast->type == NODE_SUBSHELL)
		status = exec_subshell(&ast->data.subshell, ctx);
	return (status);
}
