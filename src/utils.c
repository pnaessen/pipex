/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 08:36:13 by pnaessen          #+#    #+#             */
/*   Updated: 2025/01/22 09:31:57 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	check_cmd(char *cmd, char **env)
{
	char	*path;
	char	**args;

	args = ft_split(cmd, ' ');
	if (!args)
		return (0);
	path = get_path(args[0], env);
	if (!path)
	{
		perror("command not found");
		ft_free(args);
		return (-1);
	}
	ft_free(args);
	free(path);
	return (1);
}

void	exec_cmd(char *cmd, char **env)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	if (!args)
		handle_error("Split failed", -1, -1);
	path = get_path(args[0], env);
	if (!path)
	{
		ft_free(args);
		ft_printf("Command not found");
		exit(EXIT_FAILURE);
	}
	execve(path, args, env);
	perror("Execve failed");
	free(path);
	ft_free(args);
	exit(EXIT_FAILURE);
}

char	*get_path_var(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

char	*create_full_path(char *path_dir, char *cmd)
{
	char	*temp_path;
	char	*full_path;

	temp_path = ft_strjoin(path_dir, "/");
	if (!temp_path)
		return (NULL);
	full_path = ft_strjoin(temp_path, cmd);
	if (!full_path)
		return (free(temp_path), NULL);
	free(temp_path);
	return (full_path);
}

char	*get_path(char *cmd, char **env)
{
	char	**paths;
	char	*path_var;
	char	*full_path;
	int		i;

	path_var = get_path_var(env);
	paths = ft_split(path_var, ':');
	if (!path_var || !paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = create_full_path(paths[i], cmd);
		if (full_path && access(full_path, F_OK) == 0)
		{
			ft_free(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free(paths);
	return (NULL);
}
