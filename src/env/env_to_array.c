/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/env.h"
#include "../../libft/libft.h"
#include <stdlib.h>

static void	free_partial_envp(char **envp, int count)
{
	while (count > 0)
		free(envp[--count]);
	free(envp);
}

static char	**fill_envp(char **envp, t_env *env)
{
	t_env	*curr;
	int		i;

	curr = env;
	i = 0;
	while (curr)
	{
		envp[i] = env_to_string(curr);
		if (!envp[i])
		{
			free_partial_envp(envp, i);
			return (NULL);
		}
		curr = curr->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

char	**env_to_envp(t_env *env)
{
	char	**envp;
	int		count;

	count = env_count(env);
	envp = (char **)malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	return (fill_envp(envp, env));
}

void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
