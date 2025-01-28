/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pn <pn@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:42:30 by pn                #+#    #+#             */
/*   Updated: 2025/01/28 20:21:26 by pn               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void execute_command(t_pipex *data, char *cmd)
{
    char    **args;
    char    *path;

    args = ft_split(cmd, ' ');
    if (!args)
    {
        free_pipex(data);
        handle_error("Split failed", -1, -1);
    }
    path = get_path(*args, data->env);
    if (!path)
    {
        ft_free(args);
        free_pipex(data);
        ft_printf("Command not found");
        exit(EXIT_FAILURE);
    }
    execve(path, args, data->env);
    free(path);
    ft_free(args);
    free_pipex(data);
    handle_error("Execve failed", -1, -1);
}

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

char	*get_path(char *cmd, char **env)
{
	char	**paths;
	char	*path_var;
	char	*full_path;
	int		i;

	path_var = get_path_var(env);
	if (!path_var)
		return (NULL);
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
