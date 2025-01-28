/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pn <pn@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 08:36:13 by pnaessen          #+#    #+#             */
/*   Updated: 2025/01/28 15:43:10 by pn               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"


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

