/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 10:06:38 by pnaessen          #+#    #+#             */
/*   Updated: 2025/01/25 15:07:14 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	redirect_last(char *output, int input_fd, int **pipes, int argc,
		pid_t *pids, int is_heredoc)
{
	int	fd;

	if (output)
	{
		if (is_heredoc)
			fd = open(output, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("Open output file failed");
			close_pipes(pipes, argc - 4);
			free_pipes(pipes, argc - 4);
			free(pids);
			exit(EXIT_FAILURE);
		}
		if (dup2(fd, STDOUT_FILENO) < 0)
			handle_error("Dup2 output failed", fd, -1);
		close(fd);
	}
	if (input_fd != -1)
	{
		if (dup2(input_fd, STDIN_FILENO) < 0)
			handle_error("Dup2 pipe input failed", input_fd, -1);
		close(input_fd);
	}
}

void	redirect_first(char *input, int output_fd, int **pipes, int argc,
		pid_t *pids)
{
	int	fd;

	if (input)
	{
		fd = open(input, O_RDONLY);
		if (fd < 0)
		{
			perror("Open input file failed");
			close_pipes(pipes, argc - 4);
			free_pipes(pipes, argc - 4);
			free(pids);
			exit(EXIT_FAILURE);
		}
		if (dup2(fd, STDIN_FILENO) < 0)
			handle_error("Dup2 input failed", fd, -1);
		close(fd);
	}
	if (output_fd != -1)
	{
		if (dup2(output_fd, STDOUT_FILENO) < 0)
			handle_error("Dup2 pipe output failed", output_fd, -1);
		close(output_fd);
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

int	**create_pipes(int num_pipes)
{
	int	**pipes;
	int	i;

	pipes = malloc(sizeof(int *) * num_pipes);
	if (!pipes)
		handle_error("Memory allocation failed", -1, -1);
	i = 0;
	while (i < num_pipes)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			free(pipes);
			handle_error("Pipe creation failed", -1, -1);
		}
		i++;
	}
	return (pipes);
}

void	free_pipes(int **pipes, int num_pipes)
{
	int	i;

	i = 0;
	while (i < num_pipes)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	close_pipes(int **pipes, int num_pipes)
{
	int	i;

	i = 0;
	while (i < num_pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	execute_command(char *cmd, char **env, int **pipes, pid_t *pids,
		int argc)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	if (!args)
	{
		free_pipes(pipes, argc - 4);
		free(pids);
		handle_error("Split failed", -1, -1);
	}
	path = get_path(*args, env);
	if (!path)
	{
		free_pipes(pipes, argc - 4);
		free(pids);
		ft_free(args);
		ft_printf("Command not found");
		exit(EXIT_FAILURE);
	}
	execve(path, args, env);
	free(path);
	free_pipes(pipes, argc - 4);
	free(pids);
	ft_free(args);
	handle_error("Execve failed", -1, -1);
}

void	parent_wait(pid_t *pids, int num_pids, int **pipes, int num_pipes)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	while (i < num_pids)
	{
		if (i == num_pids - 1)
			waitpid(pids[i], &last_status, 0);
		else
			waitpid(pids[i], &status, 0);
		i++;
	}
	free(pids);
	free_pipes(pipes, num_pipes);
	if (WIFEXITED(last_status))
		exit(WEXITSTATUS(last_status));
	else if (WIFSIGNALED(last_status))
		exit(128 + WTERMSIG(last_status));
	exit(EXIT_FAILURE);
}
void	launch_pipeline(int argc, char **argv, char **env)
{
	int		**pipes;
	pid_t	*pids;
	int		i;
	int		is_heredoc;

	is_heredoc = (ft_strncmp(argv[1], "here_doc", 9) == 0);
	pipes = create_pipes(argc - 4);
	pids = malloc(sizeof(pid_t) * (argc - 3));
	if (!pids)
	{
		free_pipes(pipes, argc - 4);
		handle_error("Memory allocation failed", -1, -1);
	}
	i = 0;
	while (i < argc - 3)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			free_pipes(pipes, argc - 4);
			free(pids);
			handle_error("Fork failed", -1, -1);
		}
		if (pids[i] == 0)
		{
			if (i == 0)
			{
				if (is_heredoc)
				{
					if (dup2(pipes[i][1], STDOUT_FILENO) < 0)
						handle_error("Dup2 failed", -1, -1);
					handle_here_doc(argv[2], pipes[i][1]);
					exit(EXIT_SUCCESS);
				}
				else
					redirect_first(argv[1], pipes[i][1], pipes, argc, pids);
			}
			else if (i == argc - 4)
				redirect_last(argv[argc - 1], pipes[i - 1][0], pipes, argc,
					pids, is_heredoc);
			else
				redirect_input_output(pipes[i - 1][0], pipes[i][1]);
			if (check_cmd(argv[i + 2], env) == -1)
			{
				close_pipes(pipes, argc - 4);
				free_pipes(pipes, argc - 4);
				free(pids);
				exit(127);
			}
			close_pipes(pipes, argc - 4);
			if (is_heredoc && i == 0)
				execute_command(argv[i + 3], env, pipes, pids, argc);
			else
				execute_command(argv[i + 2], env, pipes, pids, argc);
		}
		i++;
	}
	close_pipes(pipes, argc - 4);
	parent_wait(pids, argc - 3, pipes, argc - 4);
}

// void	launch_pipeline(int argc, char **argv, char **env)
// {
// 	int		**pipes;
// 	pid_t	*pids;
// 	int		i;

// 	pipes = create_pipes(argc - 4);
// 	pids = malloc(sizeof(pid_t) * (argc - 3));
// 	if (!pids)
// 	{
// 		free_pipes(pipes, argc - 4);
// 		handle_error("Memory allocation failed", -1, -1);
// 	}
// 	i = 0;
// 	while (i < argc - 3)
// 	{
// 		pids[i] = fork();
// 		if (pids[i] < 0)
// 			handle_error("Fork failed", -1, -1);
// 		if (pids[i] == 0)
// 		{
// 			if (i == 0)
// 				redirect_first(argv[1], pipes[i][1], pipes, argc, pids);
// 			else if (i == argc - 4)
// 				redirect_last(argv[argc - 1], pipes[i - 1][0], pipes, argc,
// 					pids);
// 			else
// 				redirect_input_output(pipes[i - 1][0], pipes[i][1]);
// 			if (check_cmd(argv[i + 2], env) == -1)
// 			{
// 				close_pipes(pipes, argc - 4);
// 				free_pipes(pipes, argc - 4);
// 				free(pids);
// 				exit(127);
// 			}
// 			close_pipes(pipes, argc - 4);
// 			execute_command(argv[i + 2], env, pipes, pids, argc);
// 		}
// 		i++;
// 	}
// 	close_pipes(pipes, argc - 4);
// 	parent_wait(pids, argc - 3, pipes, argc - 4);
// }

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	*t1;
	unsigned char	*t2;

	t1 = (unsigned char *)s1;
	t2 = (unsigned char *)s2;
	i = 0;
	while (t1[i] != '\0')
	{
		if (t1[i] != t2[i])
			return (t1[i] - t2[i]);
		if (t1[i + 1] == '\0' && t2[i + 1] == '\0')
			return (0);
		i++;
	}
	return (1);
}

void	handle_here_doc(char *delimiter, int pipe_fd)
{
	char	*line;
	size_t	len;

	while (1)
	{
		fprintf(stderr, "heredoc> "); // modif printf
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		len = ft_strlen(delimiter);
		if (ft_strncmp(line, delimiter, len) == 0 && (line[len] == '\n'
				|| line[len] == '\0'))
		{
			free(line);
			break ;
		}
		write(pipe_fd, line, ft_strlen(line));
		free(line);
	}
}

int	main(int argc, char **argv, char **env)
{
	if (argc < 5)
	{
		ft_printf("Usage: ./pipex file1 cmd1 cmd2 ... cmdN file2\n");
		return (EXIT_FAILURE);
	}
	launch_pipeline(argc, argv, env);
	return (EXIT_SUCCESS);
}
