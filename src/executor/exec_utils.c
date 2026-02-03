/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"
#include <stdlib.h>

t_exec_ctx	*init_exec_ctx(char **environ)
{
	t_exec_ctx	*ctx;

	ctx = (t_exec_ctx *)malloc(sizeof(t_exec_ctx));
	if (!ctx)
		return (NULL);
	ctx->env = init_env(environ);
	if (!ctx->env)
	{
		free(ctx);
		return (NULL);
	}
	ctx->last_status = 0;
	ctx->stdin_backup = -1;
	ctx->stdout_backup = -1;
	return (ctx);
}

void	cleanup_exec_ctx(t_exec_ctx *ctx)
{
	if (!ctx)
		return ;
	free_env(ctx->env);
	free(ctx);
}
