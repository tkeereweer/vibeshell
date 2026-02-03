/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
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

static char	*extract_key(char *env_line)
{
	int		i;
	char	*key;

	i = 0;
	while (env_line[i] && env_line[i] != '=')
		i++;
	key = (char *)malloc(i + 1);
	if (!key)
		return (NULL);
	i = 0;
	while (env_line[i] && env_line[i] != '=')
	{
		key[i] = env_line[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

static char	*extract_value(char *env_line)
{
	int		i;

	i = 0;
	while (env_line[i] && env_line[i] != '=')
		i++;
	if (!env_line[i])
		return (ft_strdup(""));
	return (ft_strdup(env_line + i + 1));
}

static void	append_env_node(t_env **head, t_env *new)
{
	t_env	*curr;

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

static void	process_env_line(t_env **env, char *line)
{
	t_env	*node;
	char	*key;
	char	*value;

	key = extract_key(line);
	value = extract_value(line);
	if (!key || !value)
	{
		free(key);
		free(value);
		return ;
	}
	node = env_node_new(key, value);
	free(key);
	free(value);
	if (node)
		append_env_node(env, node);
}

t_env	*init_env(char **environ)
{
	t_env	*env;
	int		i;

	env = NULL;
	i = 0;
	while (environ[i])
	{
		process_env_line(&env, environ[i]);
		i++;
	}
	return (env);
}
