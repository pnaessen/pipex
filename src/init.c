/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 13:11:51 by pnaessen          #+#    #+#             */
/*   Updated: 2025/01/21 15:59:33 by pnaessen         ###   ########lyon.fr   */
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

int	check_cmd(char *cmd, char **env)
{
	char	*path;
	char	**args;
	int		ret;

	args = ft_split(cmd, ' ');
	if (!args)
		return (0);
	path = get_path(args[0], env);
	ft_free(args);
	if (!path)
		return (0);
	ret = (access(path, F_OK | X_OK) == 0);
	free(path);
	return (ret);
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
	if (!check_cmd(argv[2], env))
		handle_error("Execve cmd1 failed", -1, -1);
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
	if (!check_cmd(argv[3], env))
		handle_error("Execve cmd2 failed", -1, -1);
	exec_cmd(argv[3], env);
}

char	*get_path(char *cmd, char **env)
{
	int		i;
	char	**paths;
	char	*path_var;
	char	*full_path;
	char	*temp_path;

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
		temp_path = ft_strjoin(paths[i], "/");
		if (!temp_path)
			return (ft_free(paths), NULL);
		full_path = ft_strjoin(temp_path, cmd);
		free(temp_path);
		if (!full_path)
			return (ft_free(paths), NULL);
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

int	main(int argc, char **argv, char **env)
{
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int status1, status2;

	if (argc != 5)
	{
		ft_printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");
		return (EXIT_FAILURE);
	}
	if (pipe(pipe_fd) == -1) // 0 pour la lecture, 1 pour l'écriture
		handle_error("Pipe failed", -1, -1);
	pid1 = fork();
	if (pid1 < 0)
		handle_error("Fork failed", pipe_fd[0], pipe_fd[1]);
	if (pid1 == 0)
		handle_child(argv, pipe_fd, env);
	pid2 = fork();
	if (pid2 < 0)
		handle_error("Fork failed", pipe_fd[0], pipe_fd[1]);
	if (pid2 == 0)
		handle_parent(argv, pipe_fd, env);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		exit(WEXITSTATUS(status2));
	else if (WIFSIGNALED(status2))
		exit(128 + WTERMSIG(status2));
	else
		exit(EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
