/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:41:31 by pn                #+#    #+#             */
/*   Updated: 2025/01/29 08:49:07 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	redirect_last(t_pipex *data, int i)
{
	int	fd;

	if (data->argv[data->argc - 1])
	{
		if (data->is_heredoc)
			fd = open(data->argv[data->argc - 1], O_WRONLY | O_CREAT | O_APPEND,
					0644);
		else
			fd = open(data->argv[data->argc - 1], O_WRONLY | O_CREAT | O_TRUNC,
					0644);
		if (fd < 0)
			handle_fd_fail("Open output file failed", data);
		if (dup2(fd, STDOUT_FILENO) < 0)
			handle_error("Dup2 output failed", fd, -1);
		close(fd);
	}
	if (data->pipes[i - 1][0] != -1)
	{
		if (dup2(data->pipes[i - 1][0], STDIN_FILENO) < 0)
			handle_error("Dup2 pipe input failed", data->pipes[i - 1][0], -1);
		close(data->pipes[i - 1][0]);
	}
}

void	redirect_first(t_pipex *data, int i)
{
	int	fd;

	if (data->argv[1])
	{
		fd = open(data->argv[1], O_RDONLY);
		if (fd < 0)
		{
			perror("Open input file failed");
			close_pipes(data->pipes, data->num_pipes);
			free_pipex(data);
			exit(EXIT_FAILURE);
		}
		if (dup2(fd, STDIN_FILENO) < 0)
			handle_error("Dup2 input failed", fd, -1);
		close(fd);
	}
	if (data->pipes[i][1] != -1)
	{
		if (dup2(data->pipes[i][1], STDOUT_FILENO) < 0)
			handle_error("Dup2 pipe output failed", data->pipes[i][1], -1);
		close(data->pipes[i][1]);
	}
}

void	redirect_input_output(int input_fd, int output_fd)
{
	if (input_fd != -1)
	{
		if (dup2(input_fd, STDIN_FILENO) < 0)
			handle_error("Dup2 pipe input failed", input_fd, -1);
		close(input_fd);
	}
	if (output_fd != -1)
	{
		if (dup2(output_fd, STDOUT_FILENO) < 0)
			handle_error("Dup2 pipe output failed", output_fd, -1);
		close(output_fd);
	}
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
