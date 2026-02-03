/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_logical.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"

int	exec_and(t_binop_node *and_node, t_exec_ctx *ctx)
{
	int	status;

	status = execute_ast(and_node->left, ctx);
	ctx->last_status = status;
	if (status == 0)
		status = execute_ast(and_node->right, ctx);
	return (status);
}

int	exec_or(t_binop_node *or_node, t_exec_ctx *ctx)
{
	int	status;

	status = execute_ast(or_node->left, ctx);
	ctx->last_status = status;
	if (status != 0)
		status = execute_ast(or_node->right, ctx);
	return (status);
}
