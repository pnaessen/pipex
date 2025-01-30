/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 15:37:28 by pn                #+#    #+#             */
/*   Updated: 2025/01/29 08:51:43 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	launch_pipeline(int argc, char **argv, char **env)
{
	t_pipex	*data;
	int		i;

	data = init_pipex(argc, argv, env);
	data->pipes = allocate_pipes(data);
	if (!data->pipes)
	{
		free(data);
		handle_error("Pipe creation failed", -1, -1);
	}
	data->pids = malloc(sizeof(pid_t) * data->num_pids);
	if (!data->pids)
	{
		close_pipes(data->pipes, data->num_pipes);
		free_pipex(data);
		handle_error("Memory allocation failed", -1, -1);
	}
	i = -1;
	while (++i < data->num_pids)
		handle_fork(data, i);
	close_pipes(data->pipes, data->num_pipes);
	parent_wait(data);
}

t_pipex	*init_pipex(int argc, char **argv, char **env)
{
	t_pipex	*data;

	data = malloc(sizeof(t_pipex));
	if (!data)
		handle_error("Memory allocation failed", -1, -1);
	data->argc = argc;
	data->argv = argv;
	data->env = env;
	data->num_pipes = argc - 4;
	data->num_pids = argc - 3;
	data->is_heredoc = (ft_strncmp(argv[1], "here_doc", 9) == 0);
	return (data);
}

void	free_pipex(t_pipex *data)
{
	int	i;

	if (!data)
		return ;
	if (data->pipes)
	{
		i = 0;
		while (i < data->num_pipes)
		{
			if (data->pipes[i])
				free(data->pipes[i]);
			i++;
		}
		free(data->pipes);
	}
	if (data->pids)
		free(data->pids);
	free(data);
}

int	**allocate_pipes(t_pipex *data)
{
	int	**pipes;
	int	i;

	pipes = malloc(sizeof(int *) * data->num_pipes);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < data->num_pipes)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			while (--i >= 0)
				free(pipes[i]);
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	handle_fd_fail(char *str, t_pipex *data)
{
	perror(str);
	close_pipes(data->pipes, data->num_pipes);
	free_pipex(data);
	exit(EXIT_FAILURE);
}
