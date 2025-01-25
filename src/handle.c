/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 08:38:19 by pnaessen          #+#    #+#             */
/*   Updated: 2025/01/25 13:53:37 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	handle_error(const char *str, int fd1, int fd2)
{
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
	perror(str);
	exit(EXIT_FAILURE);
}

void	handle_child(char **argv, int pipe_fd[2], char **env)
{
	int	file1;

	close(pipe_fd[0]);
	file1 = open(argv[1], O_RDONLY);
	if (file1 < 0)
		handle_error("Error opening file1", file1, pipe_fd[1]);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		handle_error("dup2 pipe -> STDOUT failed", file1, pipe_fd[1]);
	if (dup2(file1, STDIN_FILENO) == -1)
		handle_error("dup2 file1 -> STDIN failed", file1, pipe_fd[1]);
	close(file1);
	close(pipe_fd[1]);
	if (check_cmd(argv[2], env) == -1)
		exit(127);
	exec_cmd(argv[2], env);
}

void	handle_parent(char **argv, int pipe_fd[2], char **env)
{
	int	file2;

	close(pipe_fd[1]);
	file2 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file2 < 0)
		handle_error("Error opening file2", file2, pipe_fd[0]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		handle_error("dup2 pipe -> STDIN failed", file2, pipe_fd[0]);
	if (dup2(file2, STDOUT_FILENO) == -1)
		handle_error("dup2 file2 -> STDOUT failed", file2, pipe_fd[0]);
	close(file2);
	close(pipe_fd[0]);
	if (check_cmd(argv[3], env) == -1)
		exit(127);
	exec_cmd(argv[3], env);
}
