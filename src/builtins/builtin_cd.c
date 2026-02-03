/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/builtins.h"
#include "../../libft/libft.h"
#include <unistd.h>
#include <stdio.h>

static char	*get_target_dir(char **argv, t_exec_ctx *ctx)
{
	char	*target;

	if (!argv[1])
	{
		target = env_get(ctx->env, "HOME");
		if (!target)
		{
			fprintf(stderr, "minishell: cd: HOME not set\n");
			return (NULL);
		}
		return (target);
	}
	if (ft_strcmp(argv[1], "-") == 0)
	{
		target = env_get(ctx->env, "OLDPWD");
		if (!target)
		{
			fprintf(stderr, "minishell: cd: OLDPWD not set\n");
			return (NULL);
		}
		return (target);
	}
	return (argv[1]);
}

static void	update_pwd_vars(t_exec_ctx *ctx, char *old_pwd)
{
	char	new_pwd[4096];

	if (old_pwd)
		env_set(&ctx->env, "OLDPWD", old_pwd);
	if (getcwd(new_pwd, sizeof(new_pwd)))
		env_set(&ctx->env, "PWD", new_pwd);
}

int	builtin_cd(char **argv, t_exec_ctx *ctx)
{
	char	*target;
	char	old_pwd[4096];

	if (!getcwd(old_pwd, sizeof(old_pwd)))
		old_pwd[0] = '\0';
	target = get_target_dir(argv, ctx);
	if (!target)
		return (1);
	if (chdir(target) != 0)
	{
		fprintf(stderr, "minishell: cd: %s: ", target);
		perror("");
		return (1);
	}
	if (old_pwd[0])
		update_pwd_vars(ctx, old_pwd);
	else
		update_pwd_vars(ctx, NULL);
	return (0);
}
