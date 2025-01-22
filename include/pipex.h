/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 13:02:08 by pnaessen          #+#    #+#             */
/*   Updated: 2025/01/22 08:41:22 by pnaessen         ###   ########lyon.fr   */
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

#endif