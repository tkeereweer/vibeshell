/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <stdlib.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

t_env	*init_env(char **environ);
char	*env_get(t_env *env, char *key);
int		env_set(t_env **env, char *key, char *value);
int		env_unset(t_env **env, char *key);
void	free_env(t_env *env);
t_env	*env_node_new(char *key, char *value);
t_env	*env_find(t_env *env, char *key);
int		env_count(t_env *env);
char	*env_to_string(t_env *node);

#endif
