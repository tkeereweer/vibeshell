/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	free_redirect_list(t_redirect *redir)
{
	t_redirect	*temp;

	while (redir)
	{
		temp = redir->next;
		if (redir->file)
			free(redir->file);
		free(redir);
		redir = temp;
	}
}

void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == NODE_COMMAND)
	{
		free_argv(node->data.cmd.argv);
		free_redirect_list(node->data.cmd.redirects);
	}
	else if (node->type == NODE_SUBSHELL)
	{
		free_ast(node->data.subshell.child);
		free_redirect_list(node->data.subshell.redirects);
	}
	else
	{
		free_ast(node->data.binop.left);
		free_ast(node->data.binop.right);
	}
	free(node);
}
