/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 10:06:38 by pnaessen          #+#    #+#             */
/*   Updated: 2025/01/22 13:36:41 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **env)
{
	int		pipe_fd[2];
	pid_t	*pid;
	int		i;

	i = 0;
	if (argc < 5)
	{
		ft_printf("Usage: ./pipex file1 cmd1 cmd2 ... file2\n");
		return (EXIT_FAILURE);
	}
	init_pipes(pipe_fd);
	pid = malloc(sizeof(pid_t) * argc - 3);
	if (!(*pid))
		return (NULL);
	pid[i++] = fork();
	if (pid[i] < 0)
		handle_error("Fork failed", pipe_fd[0], pipe_fd[1]);
	if (pid[i] == 0)
		handle_first_child(argv[i + 2], pipe_fd, env);
	while (i < (argc - 4))
	{
		pid[i] = fork();
		if (pid[i] < 0)
			handle_error("Fork failed", pipe_fd[0], pipe_fd[1]);
		if (pid[i] == 0)
			handle_cmd(argv[i + 2], pipe_fd, env);
		i++;
	}
	pid[i] = fork();
	if (pid[i] < 0)
		handle_error("Fork failed", pipe_fd[0], pipe_fd[1]);
	if (pid[i] == 0)
		handle_last_child(argv[i + 2], pipe_fd, env);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	wait_child(pid, argc);
	return (EXIT_SUCCESS);
}

void	wait_child(pid_t **pid, int argc)
{
	int	i;
	int	max;
	int	status1;
	int	status2;

	i = 0;
	max = argc - 4;
	while (i < max)
	{
		waitpid(pid[i], &status1, 0);
		i++;
	}
	waitpid(pid[i], &status2, 0);
	if (WIFEXITED(status2))
		exit(WEXITSTATUS(status2));
	else if (WIFSIGNALED(status2))
		exit(128 + WTERMSIG(status2));
	exit(EXIT_FAILURE);
}

void	init_pipes(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
		handle_error("Pipe failed", -1, -1);
}
