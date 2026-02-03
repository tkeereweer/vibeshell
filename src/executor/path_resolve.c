/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolve.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeereweer <tkeereweer@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 10:00:00 by tkeereweer       #+#    #+#             */
/*   Updated: 2026/02/03 10:00:00 by tkeereweer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/executor.h"
#include "../../libft/libft.h"
#include <unistd.h>
#include <stdlib.h>

static int	is_executable(char *path)
{
	if (access(path, F_OK) == 0)
	{
		if (access(path, X_OK) == 0)
			return (1);
	}
	return (0);
}

static char	*try_direct_path(char *command)
{
	if (ft_strchr(command, '/'))
	{
		if (is_executable(command))
			return (ft_strdup(command));
	}
	return (NULL);
}

static char	*build_path(char *dir, char *command)
{
	char	*temp;
	char	*path;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	path = ft_strjoin(temp, command);
	free(temp);
	return (path);
}

static char	*search_in_path(char **dirs, char *command)
{
	char	*full_path;
	int		i;

	i = 0;
	while (dirs[i])
	{
		full_path = build_path(dirs[i], command);
		if (!full_path)
		{
			i++;
			continue ;
		}
		if (is_executable(full_path))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*resolve_path(char *command, t_env *env)
{
	char	*path_env;
	char	**dirs;
	char	*result;

	result = try_direct_path(command);
	if (result)
		return (result);
	path_env = env_get(env, "PATH");
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	result = search_in_path(dirs, command);
	ft_free_split(dirs);
	return (result);
}
