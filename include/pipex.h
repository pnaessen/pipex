/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 13:02:08 by pnaessen          #+#    #+#             */
/*   Updated: 2025/01/23 13:31:36 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

/////////////////init.c/////////////////
void	init_pipes(int pipe_fd[2]);
void	wait_children(pid_t pid1, pid_t pid2);

///////////////////utils.c//////////////
int		check_cmd(char *cmd, char **env);
void	exec_cmd(char *cmd, char **env);
char	*get_path_var(char **env);
char	*create_full_path(char *path_dir, char *cmd);
char	*get_path(char *cmd, char **env);

/////////////////handle.c////////////////
void	handle_error(const char *str, int fd1, int fd2);
void	handle_child(char **argv, int pipe_fd[2], char **env);
void	handle_parent(char **argv, int pipe_fd[2], char **env);

////////////////////////////init_bonus//////////////////
void	redirect_input_output(char *input, char *output, int input_fd,
			int output_fd);
int		**create_pipes(int num_pipes);
void	free_pipes(int **pipes, int num_pipes);
void	close_pipes(int **pipes, int num_pipes);
void	execute_command(char *cmd, char **env, int input_fd, int output_fd);
void	parent_wait(pid_t *pids, int num_pids, int **pipes, int num_pipes);
void	launch_pipeline(int argc, char **argv, char **env);

#endif