/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 13:11:51 by pnaessen          #+#    #+#             */
/*   Updated: 2025/01/23 11:42:23 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_pipes(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
		handle_error("Pipe failed", -1, -1);
}

void	wait_children(pid_t pid1, pid_t pid2)
{
	int	status1;
	int	status2;

	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		exit(WEXITSTATUS(status2));
	else if (WIFSIGNALED(status2))
		exit(128 + WTERMSIG(status2));
	exit(EXIT_FAILURE);
}

// int	main(int argc, char **argv, char **env)
// {
// 	int		pipe_fd[2];
// 	pid_t	pid1;
// 	pid_t	pid2;

// 	if (argc != 5)
// 	{
// 		ft_printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");
// 		return (EXIT_FAILURE);
// 	}
// 	init_pipes(pipe_fd);
// 	pid1 = fork();
// 	if (pid1 < 0)
// 		handle_error("Fork failed", pipe_fd[0], pipe_fd[1]);
// 	if (pid1 == 0)
// 		handle_child(argv, pipe_fd, env);
// 	pid2 = fork();
// 	if (pid2 < 0)
// 		handle_error("Fork failed", pipe_fd[0], pipe_fd[1]);
// 	if (pid2 == 0)
// 		handle_parent(argv, pipe_fd, env);
// 	close(pipe_fd[0]);
// 	close(pipe_fd[1]);
// 	wait_children(pid1, pid2);
// 	return (EXIT_SUCCESS);
// }
