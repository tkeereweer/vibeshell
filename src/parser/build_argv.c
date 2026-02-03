/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 08:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../libft/libft.h"

void	fill_argv_array(struct s_parse_ctx *ctx, char **argv, int count)
{
	int	i;

	i = 0;
	while (i < count && ctx->curr && ctx->curr->type == TOKEN_WORD)
	{
		argv[i] = ft_strdup(ctx->curr->value);
		if (!argv[i])
		{
			free_argv(argv);
			ctx->error = 1;
			return ;
		}
		advance_token(ctx);
		i++;
	}
}

char	**build_argv(struct s_parse_ctx *ctx, int *count)
{
	char	**argv;

	*count = count_words(ctx);
	if (*count == 0)
		return (NULL);
	argv = alloc_argv_array(*count);
	if (!argv)
	{
		ctx->error = 1;
		return (NULL);
	}
	fill_argv_array(ctx, argv, *count);
	if (ctx->error)
		return (NULL);
	return (argv);
}
