/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
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

static int	env_update(t_env *node, char *value)
{
	char	*new_value;

	new_value = ft_strdup(value);
	if (!new_value)
		return (0);
	free(node->value);
	node->value = new_value;
	return (1);
}

static void	env_append(t_env **env, t_env *new)
{
	t_env	*curr;

	if (!*env)
	{
		*env = new;
		return ;
	}
	curr = *env;
	while (curr->next)
		curr = curr->next;
	curr->next = new;
}

int	env_set(t_env **env, char *key, char *value)
{
	t_env	*node;
	t_env	*new;

	node = env_find(*env, key);
	if (node)
		return (env_update(node, value));
	new = env_node_new(key, value);
	if (!new)
		return (0);
	env_append(env, new);
	return (1);
}
