/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 13:02:08 by pnaessen          #+#    #+#             */
/*   Updated: 2025/01/29 08:49:40 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipex
{
	int		**pipes;
	pid_t	*pids;
	int		argc;
	char	**argv;
	char	**env;
	int		is_heredoc;
	int		num_pipes;
	int		num_pids;
}			t_pipex;

void		launch_pipeline(int argc, char **argv, char **env);
t_pipex		*init_pipex(int argc, char **argv, char **env);
void		free_pipex(t_pipex *data);
int			**allocate_pipes(t_pipex *data);

void		setup_first_child(t_pipex *data, int i);
void		execute_child(t_pipex *data, int i);
void		handle_fork(t_pipex *data, int i);
void		handle_here_doc(t_pipex *data, int pipe_fd);

void		redirect_first(t_pipex *data, int i);
void		redirect_last(t_pipex *data, int i);
void		redirect_input_output(int input_fd, int output_fd);

void		execute_command(t_pipex *data, char *cmd);
int			check_cmd(char *cmd, char **env);
char		*get_path(char *cmd, char **env);

void		free_pipes(int **pipes, int num_pipes);
void		close_pipes(int **pipes, int num_pipes);
void		parent_wait(t_pipex *data);

void		handle_error(const char *str, int fd1, int fd2);
void		handle_fd_fail(char *str, t_pipex *data);

char		*get_path_var(char **env);
char		*create_full_path(char *path_dir, char *cmd);

#endif