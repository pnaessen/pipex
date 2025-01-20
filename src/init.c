/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 13:11:51 by pnaessen          #+#    #+#             */
/*   Updated: 2025/01/20 17:26:30 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **env)
{
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;

	// char *cmd = "cat";
	// cmd = get_path(cmd, env);
	if (argc != 5)
	{
		ft_printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");
		return (EXIT_FAILURE);
	}
	if (pipe(pipe_fd) == -1) // 0 pour la lecture 1 pour ecriture
	{
		perror("Pipe failed");
		return (EXIT_FAILURE);
	}
	pid1 = fork();
	if (pid1 < 0)
	{
		perror("Fork failed");
		return (EXIT_FAILURE);
	}
	if (pid1 == 0)
		handle_child(argv, pipe_fd, env);
	pid2 = fork();
	if (pid2 < 0)
	{
		perror("Fork failed");
		return (EXIT_FAILURE);
	}
	if (pid2 == 0)
		handle_parent(argv, pipe_fd, env);
	waitpid(pid1, NULL, 0);
	return (EXIT_SUCCESS);
}

void	handle_child(char **argv, int pipe_fd[2], char **env)
{
	int	file1;

	close(pipe_fd[0]);
	file1 = open(argv[1], O_RDONLY);
	if (file1 < 0)
	{
		perror("Error opening file1");
		exit(EXIT_FAILURE);
	}
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
	{
		close(pipe_fd[1]);
		close(file1);
		perror("dup2 pipe -> STDOUT failed");
		exit(EXIT_FAILURE);
	}
	if (dup2(file1, STDIN_FILENO) == -1)
	{
		close(pipe_fd[1]);
		close(file1);
		perror("dup2 file1 -> STDIN failed");
		exit(EXIT_FAILURE);
	}
	close(file1);
	close(pipe_fd[1]);
	if (!check_cmd(argv[2]))
		perror("Execve cmd1 failed");
	exec_cmd(argv[2], env);
}

void	handle_parent(char **argv, int pipe_fd[2], char **env)
{
	int	file2;

	close(pipe_fd[1]);
	file2 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file2 < 0)
	{
		perror("Error opening file2");
		exit(EXIT_FAILURE);
	}
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		close(file2);
		close(pipe_fd[0]);
		perror("dup2 pipe -> STDIN failed");
		exit(EXIT_FAILURE);
	}
	if (dup2(file2, STDOUT_FILENO) == -1)
	{
		close(file2);
		close(pipe_fd[0]);
		perror("dup2 file2 -> STDOUT failed");
		exit(EXIT_FAILURE);
	}
	close(file2);
	close(pipe_fd[0]);
	if (!check_cmd(argv[4]))
		perror("Execve cmd2 failed");
	exec_cmd(argv[4], env);
	exit(EXIT_FAILURE);
}

char	*get_path(char *cmd, char **env)
{
	int		i;
	char	**paths;
	char	*path_var;
	char	*full_path;

	i = 0;
	path_var = NULL;
	full_path = NULL;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path_var = env[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = malloc(ft_strlen(paths[i]) + ft_strlen(cmd) + 2);
		if (!full_path)
			return (ft_free(paths), NULL);
		strcpy(full_path, paths[i]);
		strcat(full_path, "/");
		strcat(full_path, cmd);
		if (access(full_path, F_OK) == 0)
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
