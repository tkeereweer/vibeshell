/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
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

t_env	*env_node_new(char *key, char *value)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	if (!node->key || !node->value)
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

int	env_count(t_env *env)
{
	int		count;
	t_env	*curr;

	count = 0;
	curr = env;
	while (curr)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

char	*env_to_string(t_env *node)
{
	char	*result;
	char	*temp;

	temp = ft_strjoin(node->key, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, node->value);
	free(temp);
	return (result);
}
