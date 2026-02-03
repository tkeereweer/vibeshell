/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

t_redir_type	token_to_redir(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	if (type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	if (type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	if (type == TOKEN_REDIR_HEREDOC)
		return (REDIR_HEREDOC);
	return (REDIR_IN);
}

t_redirect	*create_redirect(t_token_type type, char *file)
{
	t_redirect	*redir;

	redir = (t_redirect *)malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->type = token_to_redir(type);
	redir->file = ft_strdup(file);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->heredoc_fd = -1;
	redir->next = NULL;
	return (redir);
}

void	append_redirect(t_redirect **head, t_redirect *new)
{
	t_redirect	*curr;

	if (!*head)
	{
		*head = new;
		return ;
	}
	curr = *head;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
}

t_redirect	*parse_single_redir(t_parse_ctx *ctx)
{
	t_token_type	type;
	t_redirect		*redir;

	type = ctx->curr->type;
	advance_token(ctx);
	if (!ctx->curr || ctx->curr->type != TOKEN_WORD)
	{
		syntax_error("newline");
		ctx->error = 1;
		return (NULL);
	}
	redir = create_redirect(type, ctx->curr->value);
	if (!redir)
	{
		ctx->error = 1;
		return (NULL);
	}
	advance_token(ctx);
	return (redir);
}

t_redirect	*parse_redirections(t_parse_ctx *ctx)
{
	t_redirect	*head;
	t_redirect	*redir;

	head = NULL;
	while (ctx->curr && is_redir_token(ctx->curr->type))
	{
		redir = parse_single_redir(ctx);
		if (!redir)
		{
			free_redirect_list(head);
			return (NULL);
		}
		append_redirect(&head, redir);
	}
	return (head);
}
